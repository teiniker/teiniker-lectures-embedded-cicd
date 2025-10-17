# Using Docker Containers

On our Linux VM the **Docker Engine** (dockerd) is not started automatically to conserve 
resources. Enable and start it with systemd:

```
$ sudo systemctl start docker
$ sudo systemctl status docker

$ docker version
$ docker info

$ sudo systemctl stop docker
```

A **Docker Image** is a read-only template that contains an application 
and all of its dependencies, configuration, and metadata. 
Images are built from **Dockerfiles** or pulled from a registry (**Docker Hub**) 
and are identified by a repository name and an optional tag (repo:tag, 
default tag is `latest`). 

The following commands are used in the context of Docker images:

```
# docker image help
Commands:
  build       Build an image from a Dockerfile
  history     Show the history of an image
  import      Import the contents from a tarball to create a filesystem image
  inspect     Display detailed information on one or more images
  load        Load an image from a tar archive or STDIN
  ls          List images
  prune       Remove unused images
  pull        Pull an image or a repository from a registry
  push        Push an image or a repository to a registry
  rm          Remove one or more images
  save        Save one or more images to a tar archive (streamed to STDOUT by default)
  tag         Create a tag TARGET_IMAGE that refers to SOURCE_IMAGE
```


A **Docker Container** is an instance of a Docker image running as a process.
There are also container specific commands we can use:

```
# docker container help
Commands:
  attach      Attach local standard input, output, and error streams to a running container
  commit      Create a new image from a container's changes
  cp          Copy files/folders between a container and the local filesystem
  create      Create a new container
  diff        Inspect changes to files or directories on a container's filesystem
  exec        Run a command in a running container
  export      Export a container's filesystem as a tar archive
  inspect     Display detailed information on one or more containers
  kill        Kill one or more running containers
  logs        Fetch the logs of a container
  ls          List containers
  pause       Pause all processes within one or more containers
  port        List port mappings or a specific mapping for the container
  prune       Remove all stopped containers
  rename      Rename a container
  restart     Restart one or more containers
  rm          Remove one or more containers
  run         Run a command in a new container
  start       Start one or more stopped containers
  stats       Display a live stream of container(s) resource usage statistics
  stop        Stop one or more running containers
  top         Display the running processes of a container
  unpause     Unpause all processes within one or more containers
  update      Update configuration of one or more containers
  wait        Block until one or more containers stop, then print their exit codes
```

### Creating and Starting a Container

**Nginx** is a web server that can also be used as a reverse proxy, 
load balancer, mail proxy and HTTP cache.

The following Docker command creates and runs a new container with an Nginx web server: 
```bash
$ docker container run -p 8080:80 --name webhost -d nginx
```

* `docker container run`: Creates and starts a new container
* `-p 8080:80`: Port mapping flag that maps port `8080` on our host machine to port `80` inside the container
	Format: `-p <host-port>:<container-port>`
* `--name webhost`: Assigns a custom name "webhost" to the container for easier identification and management
* `-d`: Runs the container in **detached mode** (in the background), so it doesn't block your terminal
* `nginx`: The Docker image to use (pulls the official Nginx image from Docker Hub if not already available locally)


What this single command does:
* Downloads the Nginx image (if not already present)
* Creates a new container named "webhost"
* Starts an Nginx web server inside the container
* Maps port 80 from the container to port `8080` on our host
* Runs the container in the background

We can **list all downloaded images**:

```bash
$ docker image ls -a
REPOSITORY   TAG       IMAGE ID       CREATED      SIZE
nginx        latest    07ccdb783875   8 days ago   160MB
```

We can **list all containers**:

```bash
$ docker container ls -a
CONTAINER ID   IMAGE     COMMAND                  CREATED         STATUS         PORTS                                     NAMES
8a38dbf20ee7   nginx     "/docker-entrypoint.…"   7 minutes ago   Up 7 minutes   0.0.0.0:8080->80/tcp, [::]:8080->80/tcp   webhost
```

Using **curl** we can **access the web server** running inside the container:

```bash
$ curl http://localhost:8080
```

We can **stop the container** from running:

```bash
$ docker container stop webhost

$ docker container ls -a
CONTAINER ID   IMAGE     COMMAND                  CREATED         STATUS                     PORTS     NAMES
8a38dbf20ee7   nginx     "/docker-entrypoint.…"   9 minutes ago   Exited (0) 2 seconds ago             webhost
```

And we can **remove the container** from the list:

```bash
$ docker container rm webhost
```

Note that there is still the **Docker image** even if we removed the container.

```bash
$ docker image ls -a
REPOSITORY   TAG       IMAGE ID       CREATED      SIZE
nginx        latest    07ccdb783875   8 days ago   160MB
```

In fact, we can **create any number of containers from a single image**:

```bash
$ docker container run -p 8070:80 --name webhost_1 -d nginx
$ docker container run -p 8090:80 --name webhost_2 -d nginx

$ docker container ls -a
CONTAINER ID   IMAGE     COMMAND                  CREATED          STATUS          PORTS                                     NAMES
38965c1288bd   nginx     "/docker-entrypoint.…"   9 seconds ago    Up 9 seconds    0.0.0.0:8090->80/tcp, [::]:8090->80/tcp   webhost_2
659d9e270d39   nginx     "/docker-entrypoint.…"   18 seconds ago   Up 17 seconds   0.0.0.0:8070->80/tcp, [::]:8070->80/tcp   webhost_1
```

Let's access the two web servers:

```bash
$ curl http://localhost:8070
$ curl http://localhost:8090
```

We can **stop and start one of the containers** again:

```bash
$ docker container stop webhost_1

$ docker container ls -a
CONTAINER ID   IMAGE     COMMAND                  CREATED         STATUS                    PORTS                                     NAMES
38965c1288bd   nginx     "/docker-entrypoint.…"   2 minutes ago   Up 2 minutes              0.0.0.0:8090->80/tcp, [::]:8090->80/tcp   webhost_2
659d9e270d39   nginx     "/docker-entrypoint.…"   2 minutes ago   Exited (0) 1 second ago                                             webhost_1

$ docker container start webhost_1

$ docker container ls -a
CONTAINER ID   IMAGE     COMMAND                  CREATED         STATUS         PORTS                                     NAMES
38965c1288bd   nginx     "/docker-entrypoint.…"   2 minutes ago   Up 2 minutes   0.0.0.0:8090->80/tcp, [::]:8090->80/tcp   webhost_2
659d9e270d39   nginx     "/docker-entrypoint.…"   2 minutes ago   Up 1 second    0.0.0.0:8070->80/tcp, [::]:8070->80/tcp   webhost_1
```

Finally, we can **stop and remove the containers and remove the image** as well:

```bash
$ docker container stop webhost_1
$ docker container rm webhost_1

$ docker container stop webhost_2
$ docker container rm webhost_2

$ docker container ls -a
CONTAINER ID   IMAGE     COMMAND   CREATED   STATUS    PORTS     NAMES

$ docker image ls -a
REPOSITORY   TAG       IMAGE ID       CREATED      SIZE
nginx        latest    07ccdb783875   8 days ago   160MB

$ docker image rm 07ccdb783875
Untagged: nginx:latest
Untagged: nginx@sha256:3b7732505933ca591ce4a6d860cb713ad96a3176b82f7979a8dfa9973486a0d6
Deleted: sha256:07ccdb7838758e758a4d52a9761636c385125a327355c0c94a6acff9babff938
Deleted: sha256:71b75b17511f67932ccf71e2046c6d1b4fe17a594134c765bf71c874dedc7027
Deleted: sha256:c76da83ebfb3e35184d1a7105d03cecfb144d07cb4dae12378392040fbd44615
Deleted: sha256:f825da5ac2d31a2c717db4fa159b6728b33a94bcb7285bd1dfebcbbe23ebd185
Deleted: sha256:31333e0f1ecf5940213f8b1ed4eb3e4d78d77fa5fb59c1cd05a6672690ed133c
Deleted: sha256:e72eb931613f8c1b5baf39f864877beee0780af9f9d20c5a02790d0146f0b012
Deleted: sha256:775e3183eed457c31a0855ac7a55b6e0cb8d40554e9524cb6d503cb2351e0b10
Deleted: sha256:1d46119d249f7719e1820e24a311aa7c453f166f714969cffe89504678eaa447

$ docker image ls -a
REPOSITORY   TAG       IMAGE ID   CREATED   SIZE
```

The `docker image rm`command removes the image whose ID is 07ccdb783875 from our 
local system.
Docker first untags any names that refer to the image (like nginx:latest), then 
deletes the actual image layers if no other image uses them.
These `Deleted:`lines show the deletion of the image layers that made up the Nginx 
image. Each layer is a separate filesystem snapshot Docker uses to efficiently 
store and reuse parts of images.



### Running Interactive Containers

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




## Docker Hub

A **Docker Repository** is a named bucket of Docker images.
A repository's name is made up of the name of the host where the image is located, 
the user account that owns the image, and a short name.
As there can be several versions of software, a repository can hold several images.
Each of the images in a repository is identified uniquely with tags.
A repository name and tag form a composite key.

**Docker Indexes** are search engines that catalog repositories.
There are several public indexes, but by default Docker is integrated with an index named **Docker Hub**: 
`https://hub.docker.com`

Docker Hub makes Docker more useful out of the box.

We can use the following command to **search the index**:

```
$ docker search alpine
+NAME                DESCRIPTION                                     STARS     OFFICIAL
alpine              A minimal Docker image based on Alpine Linux…   11400     [OK]
alpine/git          A  simple git container running in alpine li…   244       
alpine/socat        Run socat command in alpine container           112       
alpine/helm         Auto-trigger docker build for kubernetes hel…   69        
alpine/curl                                                         11        
alpine/k8s          Kubernetes toolbox for EKS (kubectl, helm, i…   63        
alpine/httpie       Auto-trigger docker build for `httpie` when …   21        
alpine/bombardier   Auto-trigger docker build for bombardier whe…   28        
alpine/terragrunt   Auto-trigger docker build for terragrunt whe…   17        
alpine/openssl      openssl                                         7         
alpine/flake8       Auto-trigger docker build for fake8 via ci c…   2         
alpine/psql         psql — The PostgreSQL Command-Line Client       4         
alpine/kubectl      Kubernetes command-line tool for managing cl…   0         
alpine/ansible      run ansible and ansible-playbook in docker      27        
alpine/semver       Docker tool for semantic versioning             4         
alpine/jmeter       run jmeter in Docker                            9         
alpine/java         Repo containing the build scripts to produce…   4         
alpine/mongosh      mongosh - MongoDB Command Line Database Tools   2         
alpine/xml          several xml tools to work on xml file as jq …   1         
alpine/mysql        mysql client                                    6         
alpine/bundle       This repository has been archived by the own…   1         
alpine/gcloud       Auto-trigger docker build for gcloud (google…   0         
alpine/cfn-nag      Auto-trigger docker build for cfn-nag when n…   0         
alpine/crane                                                        0         
alpine/dfimage      reverse Docker images into Dockerfiles          70     
```


## References
* [Docker Homepage](https://www.docker.com/)
* [Docker Engine](https://docs.docker.com/engine/)

* Jeff Nickoloff. **Docker in Action**. Manning, 2016 

* [Best practices for writing Dockerfiles](https://docs.docker.com/develop/develop-images/dockerfile_best-practices/)


*Egon Teiniker, 2025, GPL v3.0*
