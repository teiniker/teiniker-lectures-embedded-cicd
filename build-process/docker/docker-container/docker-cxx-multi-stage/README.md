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

A multi-stage build in Docker lets you use **multiple FROM statements 
in one Dockerfile**, each representing a separate stage of the build.

This leads to:

* **Smaller final images**: Only the files we need (like the compiled binary) 
    are copied into the final stage, leaving behind compilers, build tools, 
    and temporary files. This greatly reduces image size.
    Lightweight images build and upload/download more quickly, saving time 
    and bandwidth.

* **Improved security**:
    Since development tools aren’t included in the final image, there’s 
    a smaller attack surface and fewer dependencies to patch.

* **Cleaner separation of concerns**:
    We can clearly separate the build environment (with compilers, 
    libraries, and source code) from the runtime environment (just the 
    executable and essentials).


By default, the stages aren't named, and we refer to them by their integer 
number, starting with `0` for the first `FROM` instruction. 

However, we can **name our stages**, by adding an `AS <NAME>` to the 
`FROM` instruction.


We can build an image from the `Dockerfile`:

```bash
# build the Docker image with the tag "hello-cmake"
$ docker build -t hello-cmake .

# list local images to confirm the image was created
 docker image ls -a
REPOSITORY    TAG       IMAGE ID       CREATED         SIZE
hello-cmake   latest    a4a636f78a60   2 minutes ago   8.34MB

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
Note that the **final image is now only 8.34MB**.

To tidy up, remove the created image:

```bash
# remove the local image
$ docker image rm hello-cmake

```


## References

* [Multi-stage builds](http://docs.docker.com/build/building/multi-stage/)

* Jeff Nickoloff. **Docker in Action**. Manning, 2016 

*Egon Teiniker, 2025, GPL v3.0*
