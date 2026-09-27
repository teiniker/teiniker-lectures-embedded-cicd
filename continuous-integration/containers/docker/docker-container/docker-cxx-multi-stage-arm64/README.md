# Building a Multi-Stage Docker Image

If we want to keep the final image smaller, we can use a **multi-stage** build:
compile in one stage, copy the binary to a clean Alpine runtime image.

## Dockerfile

Here is the modified Dockerfile:

```dockerfile
# Build Stage -------------------------------------------------------

# Compile the application using Alpine with build tools
FROM alpine:latest AS build

# Install compiler, make and cmake (no cache to keep image small)
RUN apk add --no-cache g++ make cmake

# Set working directory and copy source code
WORKDIR /app
COPY . .

# Create a separate build directory, configure and build the project
RUN mkdir -p build && cd build && cmake .. && make

# Runtime Stage -----------------------------------------------------

# Use a minimal Alpine runtime image for the final image
FROM alpine:latest

# Set runtime working directory
WORKDIR /app

# Copy the built binary from the build stage into the runtime image
COPY --from=build /app/build/hello .

# Run the application by default
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

Now, let's build a multi-platform Docker image and run a container 
using the QEMU emulator on the host system:

```bash
$ docker buildx build --platform linux/arm64 -t hello-cmake:raspi5 --load .

$ docker image ls -a
REPOSITORY          TAG               IMAGE ID       CREATED         SIZE
hello-cmake         raspi5            91b20559b9a6   12 seconds ago  8.58MB
moby/buildkit       buildx-stable-1   9403964920cf   3 weeks ago     227MB
tonistiigi/binfmt   latest            c97f15e717f7   7 weeks ago     83.3MB

# Run container on x86-64 using QEMU 
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

## Deploy on Raspberry Pi 

We save the image in a tarball and deploy it to a Raspberry Pi:

```bash
$ docker save -o hello-cmake.tar hello-cmake

# Copy image file to Raspi5
$ scp ./hello-cmake.tar student@<raspi-ip-address>:/home/student/Downloads
```

Finally, we run the container on the Raspberry Pi:
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

To tidy up, remove the created image:

```bash
# remove the local image
$ docker image rm hello-cmake:raspi5
```


## References

* [Multi-stage builds](http://docs.docker.com/build/building/multi-stage/)

*Egon Teiniker, 2025, GPL v3.0*
