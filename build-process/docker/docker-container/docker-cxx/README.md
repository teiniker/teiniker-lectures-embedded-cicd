# Create a Docker Image 

In this example we build a Docker image that compiles and runs a C++ program 
using CMake. All necessary steps are specified in the Dockerfile.


## Dockerfile

A Dockerfile is a text file that contains a series of instructions describing 
how to build a Docker image.

Each instruction (like FROM, RUN, COPY, CMD) defines a step, such as choosing 
a base image, installing dependencies, or setting the default command to run.

When Docker builds the image, it executes these steps in order, creating a 
layered, reusable, and portable environment that can run consistently on 
any system with Docker installed.

```dockerfile
# Use a small base image
FROM alpine:latest

# Install C++ toolchain and build tools (no cache to keep image small)
RUN apk add --no-cache g++ make cmake

# Set the application directory
WORKDIR /app

# Copy all project files into the container
COPY . .

# Create a build directory, run CMake and build the project
RUN mkdir -p build && cd build && cmake .. && make

# Ensure the container starts in the build output directory
WORKDIR /app/build

# Default command: run the built executable
CMD ["./hello"]
```

The Alpine Linux image uses the **Alpine Package Keeper (apk)** package manager.
apk is lightweight, fast, and designed specifically for Alpine’s minimal 
footprint. It’s used to install, update, and remove software packages inside 
Alpine-based containers.

* **Install a package**: `apk add g++`
* **Remove a package**: `apk del g++`
* **Update package index**: `apk update`
* **Upgrade installed packages**: `apk upgrade`

Alpine Linux uses the **Alpine Package** format `.apk`.
Each `.apk` file is a compressed tar archive (tar.gz or tar.xz)

We can look up apk packages in the official 
[**Alpine Linux package repositories**](https://pkgs.alpinelinux.org/packages)


We can build an image from the `Dockerfile` that can be run as a container 
just like an image pulled from Docker Hub.

```bash
# build the Docker image with the tag "hello-cmake"
$ docker build -t hello-cmake .

# list local images to confirm the image was created
$ docker image ls

# run the image (container removed after exit with --rm)
$ docker run --rm hello-cmake
Hello world using CMake!
Hello world using CMake!
Hello world using CMake!
Hello world using CMake!
Hello world using CMake!
Hello world using CMake!
Hello world using CMake!
Hello world using CMake!
Hello world using CMake!
Hello world using CMake!
```

Because we used the `--rm` flag, the container has been removed after running.

```bash
$ docker container ls -a
CONTAINER ID   IMAGE     COMMAND   CREATED   STATUS    PORTS     NAMES
```

The created image still exists locally:

```bash 
 docker image ls -a                                               
REPOSITORY    TAG       IMAGE ID       CREATED          SIZE
hello-cmake   latest    9e114682ce5c   12 seconds ago   303MB
```

## Save and Load Docker Images 

When we build an image, Docker stores its layers locally in its internal 
registry (on our machine).

The `docker save` and `docker load` commands let us export and import those 
images as flat files, so we can move them between systems without needing a 
container registry (like Docker Hub or a private registry).


To export the image as a tar archive for local storage, use:

```bash
# save the local image to a tar archive
$ docker save -o hello-cmake.tar hello-cmake

# verify the tar file was created (shows file size, owner, timestamp)
$ ll
-rw------- 1 student student 307825152 Oct 22 16:41 hello-cmake.tar

# remove the local image to simulate transferring and restoring later
$ docker image rm hello-cmake
```

We can restore the image from the tar archive by loading it back 
into Docker:

```bash 
# load the image from the tar archive
$ docker load -i hello-cmake.tar

# list images to confirm the image was restored
$ docker image ls -a
REPOSITORY    TAG       IMAGE ID       CREATED         SIZE
hello-cmake   latest    9e114682ce5c   6 minutes ago   303MB

# run the image (container removed after exit because of --rm)
$ docker run --rm hello-cmake
Hello world using CMake!
Hello world using CMake!
Hello world using CMake!
Hello world using CMake!
Hello world using CMake!
Hello world using CMake!
Hello world using CMake!
Hello world using CMake!
Hello world using CMake!
Hello world using CMake!
```

To tidy up, remove the created image and the exported tar file:

```bash
# remove the local image
$ docker image rm hello-cmake

# remove the exported tar file
$ rm -f hello-cmake.tar
```


## References
* Jeff Nickoloff. **Docker in Action**. Manning, 2016 

*Egon Teiniker, 2025, GPL v3.0*
