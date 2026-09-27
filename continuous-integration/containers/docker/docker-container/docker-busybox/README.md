# Running Interactive Containers

**BusyBox** is a tiny Linux toolkit that combines many common UNIX utilities 
(like ls, cat, sh, echo, grep, vi, etc.) into a single executable.

It’s often called “The Swiss Army Knife of Embedded Linux.”
It’s widely used in embedded systems, small devices, and in Docker minimal base images.


Download the BusyBox image from Docker Hub and run a container:

```bash
$ docker run -it --name shell busybox:latest
```

* `docker run`: Creates and starts a new container.

* `-i`: Keeps STDIN open (interactive mode).

* `-t`: Allocates a pseudo-TTY (so you get a shell-like experience).

* `--name shell`: Names the container “shell”.

* `busybox:latest`: Uses the lightweight BusyBox Linux image.

After starting the container we will drop into an **interactive shell inside the container**:
```bash
/ # ls
bin    dev    etc    home   lib    lib64  proc   root   sys    tmp    usr    var

/ # exit
```

We can now work with this interactive shell and we can **stop the container** bei typing `exit`.

The whole **size of this image** is around 4MB:

```bash
$ docker image ls
REPOSITORY   TAG       IMAGE ID       CREATED         SIZE
busybox      latest    0ed463b26dae   12 months ago   4.43MB
```

To **restart in interactive mode**, we have to attach to the container’s stdin/stdout again:

```bash
$ docker start -ai shell
```

* `start`: Starts an existing (stopped) container.
* `-a`: Attaches to the container’s stdout/stderr.
* `-i`: Keeps stdin open for interactive input.

When finished, **remove the container and its image** to clean up:

```bash
$ docker container ls -a
CONTAINER ID   IMAGE            COMMAND   CREATED          STATUS                      PORTS     NAMES
c365580fe14f   busybox:latest   "sh"      12 minutes ago   Exited (0) 12 seconds ago             shell

$ docker container rm shell

$ docker image ls -a
REPOSITORY   TAG       IMAGE ID       CREATED         SIZE
busybox      latest    0ed463b26dae   12 months ago   4.43MB

$ docker image rm 0ed463b26dae
Untagged: busybox:latest
Untagged: busybox@sha256:2f590fc602ce325cbff2ccfc39499014d039546dc400ef8bbf5c6ffb860632e7
Deleted: sha256:0ed463b26daee791b094dc3fff25edb3e79f153d37d274e5c2936923c38dac2b
Deleted: sha256:80e840de630d08a6a1e0ee30e7c8378cf1ed6a424315d7e437f54780aee6bf5a
```


## References
* Jeff Nickoloff. **Docker in Action**. Manning, 2016 


*Egon Teiniker, 2025, GPL v3.0*
