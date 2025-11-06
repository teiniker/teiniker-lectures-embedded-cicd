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

## Build a Docker Image 

We can build a image from the `Dockerfile`:

```bash
# Build the Docker image with the tag "hello-cmake"
$ docker build -t hello-cmake .

# List local images to confirm the image was created
$ docker image ls

# Show the layers and build steps of the hello-cmake image
$ docker image history hello-cmake
```

## Create Docker Container

```bash
# Run the image (container removed after exit with --rm)
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

## Clean Up 

To tidy up, remove the created image:

```bash
# remove the local image
$ docker image rm hello-cmake
```


## References

* [Multi-stage builds](http://docs.docker.com/build/building/multi-stage/)

*Egon Teiniker, 2025, GPL v3.0*
