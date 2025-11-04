# Building a Docker Image for ARM64

## Dockerfile

The Dockerfile does not need to be modified for the multi-architecture build.

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

## Docker Buildx
Docker Buildx is an extended build tool for Docker that adds powerful 
features on top of the standard docker build command.

We can build Docker images for multiple CPU platforms from one machine, 
such as: linux/amd64, linux/arm64, linux/arm/v7	

```bash
$ docker buildx create --name multiarch --use

# verify it’s the current builder
$ docker buildx ls         
```

* `docker buildx create`: Creates a new Buildx builder instance.
    - Buildx uses “builder instances” to control how and where images are built
    - A builder can be local, remote, multi-node, or connect to Docker 
        drivers like docker-container or kubernetes
    - We can have multiple builders and switch between them.

* `--name multiarch`: This assigns a name to the builder instance.
    - Without a name, Docker would auto-generate one.
    - Naming it helps us list, inspect, remove, or reuse it later:
        - `docker buildx rm multiarch`
        - `docker buildx use multiarch`

* `--use`: Immediately activates this builder and sets it as the current 
    default for future docker buildx build commands.
    Without `--use`, the builder is created but not selected.

Because the default Docker builder doesn’t support multi-architecture builds on its own.
Creating a new builder like this:
* Enables BuildKit (faster builds, caching, parallelism)
* Allows cross-platform builds (`--platform linux/arm64,linux/amd64`)
* Works with QEMU emulation for foreign architectures

* `docker buildx ls`: Docker prints a table showing:
    - what builders exist
    - which builder is currently active
    - what architectures they support
    - their status (running/stopped)
    - which driver each uses


### QEMU 

Next, we have to **install QEMU emulation support** on our machine so we can run 
ARM64 binaries (or build ARM64 Docker images) on a non-ARM64 host like x86-64.

This makes Docker able to execute ARM code transparently, which is crucial for 
docker buildx multi-architecture builds.

```bash
$ docker run --privileged --rm tonistiigi/binfmt --install arm64
```

* `--privileged`: Gives the container elevated permissions so it can modify 
    system settings. Required because it needs to add entries to `binfmt_misc` 
    (a kernel feature).

* `--rm`: Automatically deletes the container after it finishes.
    The changes persist, but the container isn’t left behind.

* `tonistiigi/binfmt`: This is the container image.
    It’s a lightweight tool created by a Docker BuildKit maintainer.
    Purpose: install CPU architecture interpreters for cross-execution.

* `--install arm64`: Installs the emulator for ARM64 only.
    - We could also do: `--install all` to install interpreters for most 
    architectures (`arm`, `arm64`, `s390x`, `ppc64le`, `riscv`, etc.).

Now, we can run ARM64 binaries directly:
```bash
$ ./some-arm64-binary
```

We can test ARM64 container images locally:
```bash
$ docker run --platform=linux/arm64 arm64v8/alpine 
```

We can build ARM64 Docker images using Buildx:
```bash
$ docker buildx build --platform linux/arm64 -t myapp:arm64 . 
```

### Build the Docker Image 

We want to build a Docker **image for ARM64** (for devices like a Raspberry Pi 5) 
on our current machine, and then load that image into our local Docker engine so 
that we can run it locally.

```bash
# Build the Docker image with the tag "hello-cmake" for linux/arm64
$ docker buildx build --platform linux/arm64 -t hello-cmake:raspi5 --load .

# List local images to confirm the image was created
$ docker image ls -a
REPOSITORY          TAG               IMAGE ID       CREATED         SIZE
hello-cmake         raspi5            be31b668a9b3   2 minutes ago   282MB
moby/buildkit       buildx-stable-1   9403964920cf   3 weeks ago     227MB
tonistiigi/binfmt   latest            c97f15e717f7   7 weeks ago     83.3MB

# Show the layers and build steps of the hello-cmake image
$ docker image history hello-cmake:raspi5
```

* `docker buildx build`: Uses Buildx, Docker’s advanced builder (BuildKit-powered).
    Supports multi-architecture builds, caching, and parallel builds.

* `--platform linux/arm64`: Tells Buildx to produce an image whose CPU/OS target is:
    - OS: Linux
    - Architecture: ARM64 (aka aarch64)

    This is what Raspberry Pi 5 uses when running a 64-bit OS.

* `-t hello-cmake:raspi5`: Tags the resulting image so Docker can refer to it by name:
    - repository: hello-cmake
    - tag: raspi5

* `--load`: Converts the built image into a format the regular Docker daemon can 
    use and loads it into the local image store. 
    - Without it, Buildx would not load the image into our local Docker
    - Buildx normally stores images in its own cache or pushes to a registry


We have built a **Docker image suitable for the Raspberry Pi** 
(linux/arm64 architecture).

To run this ARM container on a PC, we use the **QEMU emulator**:

```bash
# Run the image on x86-64 (using QEMU to simulate an linux/arm64 architecture) 
$ docker run --rm --platform=linux/arm64 hello-cmake:raspi5 
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

* `--platform=linux/arm64`: Force Docker to run this image as Linux on ARM64.

It runs a Docker container from the image `hello-cmake:raspi5`, forcing Docker 
to **execute it as an ARM64 container**, even if your machine is not ARM64 
(e.g., you're on x86-64).
If **binfmt/QEMU** is installed, the container will run under ARM emulation.


## Run Container on Raspberry Pi 

To run the container on the Raspberry Pi, we must either push the image to a registry 
or transfer it as a tarball. Here we use the **tarball approach**:

```bash
# Save image to a file
$ docker save -o hello-cmake.tar hello-cmake

# Copy image file to Raspi5
$ scp ./hello-cmake.tar student@<Raspi-IP>:/home/student/Downloads
```

Then we connect to the Raspberry Pi, load the image from the tar file, 
and start the container:

```bash
$ ssh 192.168.0.73
$ cd Downloads 
$ docker load -i hello-cmake.tar

$ docker image ls -a
REPOSITORY    TAG       IMAGE ID       CREATED         SIZE
hello-cmake   raspi5    be31b668a9b3   9 minutes ago   282MB

$ docker run --rm hello-cmake:raspi5 
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

With that, we built a Docker image on the PC and ran it as a container on the Raspberry Pi.


## References

* [Docker - Multi-platform builds](https://docs.docker.com/build/building/multi-platform/)

*Egon Teiniker, 2025, GPL v3.0*
