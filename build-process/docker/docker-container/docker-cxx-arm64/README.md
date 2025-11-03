# Building a Docker Image for ARM64

## Multi-Architecture Image Builds

Docker solves the "it works on my machine" problem by packaging applications 
and their dependencies into containers. This makes it easy to run the same 
application on different environments, such as development, testing, and production.

But containerization by itself only solves part of the problem. Containers 
share the host kernel, which means that the code that's running inside the 
container must be compatible with the host's architecture. 
This is why we can't run a `linux/amd64` container on an `arm64` host 
(without using emulation), or a Windows container on a Linux host.

Multi-platform builds solve this problem by packaging multiple variants 
of the same application into a single image. This enables us to run the 
same image on different types of hardware, such as development machines 
running `x86-64` or ARM-based Amazon EC2 instances in the cloud, without 
the need for emulation.
When building images, this lets us create a single image that can run on 
multiple platforms, such as `linux/amd64`, `linux/arm64`, and `windows/amd64`.


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
$ docker buildx version 
$ docker buildx create --name multiarch --use
$ docker buildx ls                 # verify it’s the current builder
```

```bash
$ docker run --privileged --rm tonistiigi/binfmt --install arm64
```

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
