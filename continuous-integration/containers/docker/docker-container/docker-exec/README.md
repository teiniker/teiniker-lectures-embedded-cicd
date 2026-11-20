# Docker exec

The `docker exec` command **starts an additional process inside an
already running container**. It is the tool we use to look into a
container, to debug a problem, or to use a program which is installed in
the container (like a database client).

```bash
docker exec [OPTIONS] CONTAINER COMMAND [ARG...]
```

A container is started with exactly one **main process** (defined by
`CMD` or `ENTRYPOINT` of the image, for example the Nginx server). With
`docker exec` we start more processes next to it. They run in the same
environment as the main process:
* the same **file system**,
* the same **network** (IP address, ports, `localhost`),
* the same **process namespace** (they can see the other processes),
* by default the same **user**, **working directory** and **environment
    variables** as defined by the image.

The container is **not** created or restarted. When the executed command
ends, only this process ends; the container keeps running.

Note the difference to `docker run`, which always creates a **new**
container from an image.


## Running a Single Command

We start an Nginx web server in a container named `web`:

```bash
$ docker run -d --name web -p 8080:80 nginx
```

Now we execute commands inside this container. The output is shown in
our terminal:

```bash
$ docker exec web nginx -v
nginx version: nginx/1.31.9

$ docker exec web cat /etc/os-release
PRETTY_NAME="Debian GNU/Linux 13 (trixie)"
NAME="Debian GNU/Linux"
...

$ docker exec web ls /usr/share/nginx/html
50x.html
index.html
```

The commands are executed with the programs of the **container's file
system**, not with the programs of the host. The Nginx image is based on
Debian, so we can use the usual Linux tools, but only those installed in
the image.

### Exit Codes

`docker exec` returns the exit code of the executed command, so we can
use it in scripts:

```bash
$ docker exec web ls /nope
ls: cannot access '/nope': No such file or directory
$ echo $?
2
```

If the command doesn't exist in the container, Docker can't start the
process and returns `127`:

```bash
$ docker exec web nosuchcmd
OCI runtime exec failed: exec failed: unable to start container process: exec: "nosuchcmd": executable file not found in $PATH
$ echo $?
127
```

### Commands Need a Shell for Pipes and Variables

`docker exec` starts the given program **directly**, without a shell.
Everything we type is first interpreted by the **shell on the host**:

```bash
$ docker exec web echo $HOSTNAME
debian13
```

The output is the host name of the **host**, because our local shell
replaces `$HOSTNAME` before Docker sees the command. To use a shell
inside the container, we start it explicitly with `sh -c` and use single
quotes:

```bash
$ docker exec web sh -c 'echo $HOSTNAME'
fc2f210409b7
```

Now the shell in the container expands the variable, and we get the
container's host name, which is its container ID.

The same applies to pipes (`|`) and redirections (`>`):
* `docker exec web ls /etc | wc -l`: `ls` runs in the container, `wc`
    runs on the host.
* `docker exec web sh -c 'ls /etc | wc -l'`: both run in the container.


## Interactive Shell: `-it`

The most common use of `docker exec` is to open a **shell inside the
container**:

```bash
$ docker exec -it web bash
root@fc2f210409b7:/# hostname
fc2f210409b7
root@fc2f210409b7:/# exit
exit
```

The prompt shows that we are user `root` in the container
`fc2f210409b7`. With `exit` (or `Ctrl+D`) we leave the shell. This only
ends the `bash` process; the container is still running:

```bash
$ docker container ls --filter name=web
CONTAINER ID   IMAGE   COMMAND                  CREATED          STATUS          PORTS                                     NAMES
fc2f210409b7   nginx   "/docker-entrypoint.…"   10 minutes ago   Up 10 minutes   0.0.0.0:8080->80/tcp, [::]:8080->80/tcp   web
```

For an interactive shell, we need two options:

* `-i` (`--interactive`): Keeps `stdin` open, so the process can read
    what we type. Without `-i`, the input is not passed to the container:

    ```bash
    $ echo 'echo piped; whoami' | docker exec -i web sh
    piped
    root

    $ echo 'echo piped' | docker exec web sh
    (no output)
    ```

* `-t` (`--tty`): Allocates a **pseudo terminal** (TTY) for the process.
    The shell then shows a prompt, supports line editing and `Ctrl+C`,
    and programs format their output for a terminal.

    ```bash
    $ docker exec -it web bash -c 'tty; echo $TERM'
    /dev/pts/0
    xterm
    ```

So `-it` is used for interactive work, `-i` alone for passing data into
a command (for example an SQL script, see below), and no option for a
single command whose output we only read.

`-t` requires that our own `stdin` is a terminal. In a script or a CI
pipeline, `-it` fails:

```bash
$ echo 'hostname' | docker exec -it web bash
cannot attach stdin to a TTY-enabled container because stdin is not a terminal
```

In scripts, we therefore use `-i` (or no option) instead of `-it`.

### bash or sh?

Not every image contains `bash`. Small images, for example those based
on Alpine Linux, only contain `sh`:

```bash
$ docker exec -it <container> sh
```

Some images, built `FROM scratch` or as "distroless" images, contain
**no shell at all**, only the application. Then `docker exec` can only
run the programs which exist in the image:

```bash
$ docker exec -it no-shell sh
OCI runtime exec failed: exec failed: unable to start container process: exec: "sh": executable file not found in $PATH
```

This is intended: an image without a shell and tools gives an attacker
fewer possibilities.


## Processes in the Container

The Nginx image doesn't contain the `ps` command. But we can see the
processes of the container from the **host** with `docker top`:

```bash
$ docker top web
UID      PID     PPID    C   STIME   TTY   TIME       CMD
root     20721   20688   2   08:00   ?     00:00:00   nginx: master process nginx -g daemon off;
avahi    20794   20721   0   08:00   ?     00:00:00   nginx: worker process
avahi    20795   20721   0   08:00   ?     00:00:00   nginx: worker process
...
```

Inside the container, we read the process list from `/proc`:

```bash
$ docker exec web sh -c 'for p in /proc/[0-9]*; do echo "${p#/proc/} $(tr "\0" " " < $p/cmdline)"; done'
1 nginx: master process nginx -g daemon off;
29 nginx: worker process
30 nginx: worker process
...
93 sh -c for p in /proc/[0-9]*; do ...
```

We can make two observations:
* **Process IDs:** In the container, the Nginx master process has the
    PID **1**; on the host it has the PID 20721. The container has its
    own PID namespace. Our `sh` process, started with `docker exec`,
    appears in the same list as the processes of the main command.
* **User names:** The worker processes run as UID 101, which is the user
    `nginx` in the container. On the host, UID 101 belongs to another user
    (here `avahi`), so `docker top` shows that name. Users are identified
    by their **number**, not by their name.

### Background Processes: `-d`

With `-d` (`--detach`), the command runs in the background and
`docker exec` returns immediately:

```bash
$ docker exec -d web sh -c 'sleep 30'

$ docker top web
...
root     21430   20688   25  08:00   ?     00:00:00   sh -c sleep 30
root     21437   21430   0   08:00   ?     00:00:00   sleep 30
```

### The Main Process Decides

A container runs exactly as long as its **main process**. When the main
process ends (for example with `docker stop`), all processes started
with `docker exec` are killed as well:

```bash
$ docker exec web sleep 60      # in a first terminal
$ docker stop web               # in a second terminal

# first terminal: docker exec returns with exit code 137 (killed)
```

And we can't execute a command in a stopped container:

```bash
$ docker exec web ls
Error response from daemon: container fc2f210409b7... is not running
```

In this case, we start the container again with `docker start web`.


## Options: User, Working Directory, Environment

By default, the command runs with the user, working directory and
environment variables of the image. We can change this for a single
`docker exec`:

```bash
# Default: user and working directory of the image
$ docker exec web whoami
root
$ docker exec web pwd
/

# -u, --user: run as another user (name or UID)
$ docker exec -u nginx web id
uid=101(nginx) gid=101(nginx) groups=101(nginx)

# -w, --workdir: start in another directory
$ docker exec -w /usr/share/nginx/html web pwd
/usr/share/nginx/html

# -e, --env: set an additional environment variable
$ docker exec -e GREETING=Hi web sh -c 'echo $GREETING'
Hi
```

`-u` is useful to check file permissions as the user which runs the
application, or the other way round, to get `root` in a container whose
image switches to a normal user with the `USER` instruction
(`docker exec -u root ...`).


## Changing Files in a Container

With `docker exec`, we can also **modify** the container. For example,
we replace the start page of the web server:

```bash
$ docker exec web sh -c 'echo "<h1>Hello from docker exec</h1>" > /usr/share/nginx/html/index.html'

$ curl http://localhost:8080
<h1>Hello from docker exec</h1>
```

The `docker diff` command lists all files which have been changed (`C`),
added (`A`) or deleted (`D`) compared to the image:

```bash
$ docker diff web
...
C /usr/share/nginx/html/index.html
```

The change is stored in the **writable layer** of this container. It
survives a restart (`docker stop` / `docker start`), but it is **lost**
when the container is removed. A new container starts with the original
file from the image:

```bash
$ docker rm -f web
$ docker run -d --name web -p 8080:80 nginx

$ curl -s http://localhost:8080 | grep title
<title>Welcome to nginx!</title>
```

So `docker exec` is great for **experiments and debugging**, but it is
not the way to configure a container permanently. Permanent changes
belong into the `Dockerfile` (build a new image), or into a bind mount or
volume.


## Example: Database Client in a Container

Many images contain useful client programs. The MariaDB image, for
example, contains the `mariadb` command line client. So we don't need to
install a client on the host:

```bash
$ docker run -d --name db -e MARIADB_ROOT_PASSWORD=root66 mariadb:13.0.2

# Interactive SQL session
$ docker exec -it db mariadb -u root -p
Enter password:
MariaDB [(none)]>

# Single SQL statement
$ docker exec db mariadb -u root -proot66 -e "SHOW DATABASES"

# Execute an SQL script from the host (-i passes stdin, no -t)
$ docker exec -i db mariadb -u root -proot66 < setup.sql
```

The last command shows why we sometimes need `-i` without `-t`: the
redirection `< setup.sql` is done by the host shell, and `-i` passes the
file content to the client in the container.

Since the client runs **inside** the container, it connects to
`localhost` of the container. No port mapping is needed.


## docker compose exec

In a Docker Compose project, we use `docker compose exec` with the
**service name** instead of the container name:

```bash
$ docker compose exec web ls /usr/share/nginx/html
$ docker compose exec web bash
```

There is one difference: `docker compose exec` allocates a TTY **by
default**, so we don't need `-it`. If `stdin` is not a terminal, it
switches the TTY off automatically. With `-T`, we switch it off
explicitly, which is recommended in scripts and CI pipelines:

```bash
$ docker compose exec -T web sh -c 'tty'
not a tty
```


## Summary

| Command                                  | Meaning                           |
|------------------------------------------|-----------------------------------|
| `docker exec web ls /`                   | Run a single command              |
| `docker exec web sh -c 'echo $HOME'`     | Use the shell of the container    |
| `docker exec -it web bash`               | Open an interactive shell         |
| `docker exec -i db mariadb < setup.sql`  | Pass stdin to the command         |
| `docker exec -d web sleep 30`            | Run in the background             |
| `docker exec -u nginx web id`            | Run as another user               |
| `docker exec -w /tmp web pwd`            | Start in another directory        |
| `docker exec -e KEY=value web env`       | Set an environment variable       |
| `docker compose exec web bash`           | Same for a Docker Compose service |


## References

* [Docker CLI: docker container exec](https://docs.docker.com/reference/cli/docker/container/exec/)
* [Docker CLI: docker compose exec](https://docs.docker.com/reference/cli/docker/compose/exec/)

*Egon Teiniker, 2025-2026, GPL v3.0*
