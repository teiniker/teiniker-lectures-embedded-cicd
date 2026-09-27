# Creating and Starting a Container

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


## References
* Jeff Nickoloff. **Docker in Action**. Manning, 2016 

*Egon Teiniker, 2025, GPL v3.0*
