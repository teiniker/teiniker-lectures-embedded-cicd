# Docker Bind Mounts

When we use a **bind mount**, a file or directory on the **host machine**
is mounted into a container. Both see the **same files**:
* Files inside the mounted directory are shared between host and
    container.
* Any change on the host appears in the container immediately, and the
    other way round.
* We can **edit files without rebuilding the image**.

Bind mounts are typically used for:
* **Live development:** The source code stays on the host, where we edit
    it with our IDE, and runs in the container.
* **Configuration files:** A configuration file from the host replaces
    the default configuration of an image.
* **Exchanging data:** A container reads input files from the host or
    writes results (build artifacts, logs) back to the host.

Bind mounts are similar to [volumes](../docker-volumes/). The difference
is **who manages the storage**:

|                  | Bind mount                | Volume                     |
|------------------|---------------------------|----------------------------|
| Location on host | Any path we choose        | `/var/lib/docker/volumes/` |
| Managed by       | Us (host file system)     | Docker (`docker volume`)   |
| Typical use      | Source code, config files | Database files, data       |
| Created by       | Us (or `-v`, see below)   | Docker, on first use       |
| Syntax           | `-v /abs/path:/path`      | `-v name:/path`            |

So a bind mount depends on the directory structure of the host, while a
volume is completely managed by Docker.


## Creating a Bind Mount

We create a local directory `html/` with a start page for a web server:

```bash
$ mkdir html
$ echo '<h1>Hello from the host</h1>' > html/index.html
```

Now we start an Nginx container and mount `html/` to the directory from
which Nginx serves its files:

```bash
$ docker run -d --name web -p 8080:80 -v "$(pwd)/html:/usr/share/nginx/html" nginx

$ curl http://localhost:8080
<h1>Hello from the host</h1>
```

The `-v` (`--volume`) flag has the format:

```bash
-v <host-path>:<container-path>[:<options>]
```

* `<host-path>`: The file or directory on the host. It must be an
    **absolute path** (or start with `./`, see below). `$(pwd)` is
    replaced by the shell with the current directory. The quotes protect
    paths which contain spaces.
* `<container-path>`: Where the host path appears in the container.
* `<options>`: For example `ro` for read only.

When we change the file on the host, the web server immediately delivers
the new content. There is no need to rebuild the image or restart the
container:

```bash
$ echo '<h1>Changed on the host</h1>' > html/index.html

$ curl http://localhost:8080
<h1>Changed on the host</h1>
```

### Inspecting a Bind Mount

`docker container inspect` shows the mounts of a container:

```bash
$ docker container inspect web --format '{{json .Mounts}}'
[{"Type":"bind","Source":"/home/student/.../html","Destination":"/usr/share/nginx/html","Mode":"","RW":true,"Propagation":"rprivate"}]
```

* `Type`: `bind` for a bind mount (`volume` for a volume).
* `Source`: The path on the host.
* `Destination`: The path in the container.
* `RW`: `true` if the container can write to the mount.


## A Bind Mount Hides the Image Content

The original `/usr/share/nginx/html` directory of the image contains two
files. In our container, we only see the content of our `html/`
directory:

```bash
$ docker run --rm nginx ls /usr/share/nginx/html
50x.html
index.html

$ docker exec web ls /usr/share/nginx/html
index.html
```

The bind mount is placed **on top of** the directory in the image. The
files of the image are not deleted or changed, they are only **hidden**
as long as the mount exists. A container without the mount sees the
original files again.

This is important when we mount over a directory which the application
needs, for example a directory with installed libraries.


## File Ownership and Permissions

Files created by the container in a bind mount belong to the **user of
the container process**, identified by its numeric user ID (UID). Most
images run as `root` (UID 0):

```bash
$ docker exec web sh -c 'echo "<p>from container</p>" > /usr/share/nginx/html/new.html'

$ ls -l html
-rw-rw-r-- 1 student student 29 Nov 20 08:06 index.html
-rw-r--r-- 1 root    root    22 Nov 20 08:06 new.html
```

On the host, `new.html` now belongs to `root`, so we can't edit it as a
normal user. (We can still delete it, because we own the directory.)

To avoid this, we run the container process with our own UID and GID,
using the `-u` (`--user`) option:

```bash
$ docker run --rm -u "$(id -u):$(id -g)" -v "$(pwd)/html:/data" alpine sh -c 'touch /data/mine.txt; id'
uid=1000 gid=1000 groups=1000

$ ls -l html/mine.txt
-rw-r--r-- 1 student student 0 Nov 20 08:06 html/mine.txt
```

Note that some images (like Nginx) need to start as `root`, so `-u`
doesn't work for every image.

```bash
$ docker rm -f web
```


## Read-Only Bind Mounts

If the container should only **read** the files, we add the `ro` option.
This protects the files on the host against changes by the container:

```bash
$ docker run --rm -v "$(pwd)/html:/data:ro" alpine sh -c 'cat /data/index.html; touch /data/x'
<h1>Changed on the host</h1>
touch: /data/x: Read-only file system
```

`docker container inspect` then shows `"Mode":"ro","RW":false`.

Read-only mounts are a good default for configuration files and source
code which the container doesn't need to change.


## `-v` vs. `--mount`

Docker has a second, more explicit syntax for mounts: `--mount` with
comma separated `key=value` pairs.

```bash
$ docker run --rm --mount type=bind,source="$(pwd)/html",target=/data,readonly alpine ls /data
index.html
```

Both create the same bind mount, but they behave differently if the
**host path does not exist**:

* `-v` silently **creates** an empty directory on the host, owned by
    `root`:

    ```bash
    $ docker run --rm -v "$(pwd)/missing:/data" alpine ls -ld /data
    drwxr-xr-x    2 root     root            40 Nov 20 07:06 /data

    $ ls -ld missing
    drwxr-xr-x 2 root root 40 Nov 20 08:06 missing
    ```

    A typo in the path results in an empty directory in the container,
    and the application fails later with a less obvious error.

* `--mount` reports an **error**:

    ```bash
    $ docker run --rm --mount type=bind,source="$(pwd)/missing2",target=/data alpine ls /data
    docker: Error response from daemon: invalid mount config for type "bind": bind source path does not exist: /home/student/.../missing2
    ```

`--mount` is more verbose, but easier to read and safer. `-v` is
shorter and therefore common in examples and on the command line.

### Pitfall: Relative Paths

With `-v`, the first part decides whether we get a bind mount or a
volume:

```bash
# A name: Docker creates a named VOLUME called "html" (empty)
$ docker run --rm -v html:/data alpine ls /data

$ docker volume ls
DRIVER    VOLUME NAME
local     html

# A path (starts with / or ./): a BIND MOUNT of the local html/ directory
$ docker run --rm -v ./html:/data alpine ls /data
index.html
```

So we always use an absolute path (`$(pwd)/html`) or start a relative
path with `./`.


## Mounting a Single File

We can also mount a **single file**, for example a configuration file:

```bash
$ docker run -d --name conf -v "$(pwd)/conf.txt:/conf.txt" alpine sleep 300
```

**Pitfall:** A single file mount is bound to the file itself (its
**inode**), not to its name. Many editors (and `sed -i`) don't change a
file in place, but write a new file and rename it. Then the container
still sees the **old** file:

```bash
$ echo v1 > conf.txt         # before starting the container

$ echo v2 >> conf.txt        # changes the file in place
$ docker exec conf cat /conf.txt
v1
v2

$ sed -i 's/v1/V1/' conf.txt  # writes a new file
$ docker exec conf cat /conf.txt
v1
v2

$ docker restart conf
$ docker exec conf cat /conf.txt
V1
v2
```

After a restart, the mount is created again and the container sees the
new file. If the files change often, we mount the **directory** which
contains the file instead.

```bash
$ docker rm -f conf
```


## Bind Mounts in Docker Compose

In a `docker-compose.yml` file, bind mounts are listed under `volumes`.
A path starting with `./` is relative to the directory of the
`docker-compose.yml` file:

```yaml
services:
  web:
    image: nginx
    ports:
      - "8080:80"
    volumes:
      - ./html:/usr/share/nginx/html:ro
```

Docker Compose resolves `./html` to an absolute path, so we don't need
`$(pwd)`.


## Summary

| Option                                     | Meaning                       |
|--------------------------------------------|-------------------------------|
| `-v "$(pwd)/src:/app"`                     | Bind mount a directory        |
| `-v "$(pwd)/src:/app:ro"`                  | Same, read only               |
| `-v ./src:/app`                            | Relative path (with `./`)     |
| `-v src:/app`                              | **Volume** named `src`!       |
| `--mount type=bind,source=...,target=...`  | Explicit, path must exist     |
| `-u "$(id -u):$(id -g)"`                   | Create files as our host user |


## Examples

* [docker-flask-mount](../docker-flask-mount/): Live development of a
    Flask REST service with a bind mount.


## References

* [Docker: Bind mounts](https://docs.docker.com/engine/storage/bind-mounts/)
* [Docker: Volumes](https://docs.docker.com/engine/storage/volumes/)
* Jeff Nickoloff. **Docker in Action**. Manning, 2016

*Egon Teiniker, 2025-2026, GPL v3.0*
