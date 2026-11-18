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

## Build Multi-Platform Image 

We can build a multi-platform image from the `Dockerfile`:

```bash
# Creates a new Buildx builder instance (if not already done)
$ docker buildx create --name multiarch --use

# Install QEMU emulation support on our machine (if not already done)
$ docker run --privileged --rm tonistiigi/binfmt --install arm64
```

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
$ scp ./hello-cmake.tar student@<raspi-ip-address>:/home/student/Downloads
```

Then we connect to the Raspberry Pi, load the image from the tar file, 
and start the container:

```bash
$ ssh <raspi-ip-address>
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

To tidy up, remove the created image:

```bash
# remove the local image
$ docker image rm hello-cmake:raspi5
```


## References

* [Docker - Multi-platform builds](https://docs.docker.com/build/building/multi-platform/)

*Egon Teiniker, 2025, GPL v3.0*
