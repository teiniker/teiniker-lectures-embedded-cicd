# Example: C++ Class With Google Tests   

In this example, a Docker image is built to run **Google Test** against a C++ class. 

This Docker image is intended for **development and testing of C/C++ applications**, 
not for distributing a product.

## Dockerfile

```dockerfile
# Use Alpine as the base image
FROM alpine:latest

# Install required packages: g++, make, cmake, and build tools
RUN apk add --no-cache git g++ make cmake

# Install Google Test framework
RUN git clone -q https://github.com/google/googletest.git /googletest \
  && mkdir -p /googletest/build \
  && cd /googletest/build \
  && cmake .. && make && make install \
  && cd / && rm -rf /googletest

# Set the working directory
WORKDIR /app

# Copy project files into the container
COPY ./src .

# Create a build directory and compile using CMake
RUN mkdir -p build && cd build && cmake .. && make

# Set the default working directory to build output
WORKDIR /app/build

# Default command to run the compiled program (assuming target is 'app')
CMD ["./test"]
```

We can build an image from the `Dockerfile` that can be run as a container 
just like an image pulled from Docker Hub.

```bash
# Build the Docker image with the tag "lcd-test"
$ docker build -t lcd-test .

# List local images to confirm the image was created
$ docker image ls -a

# Show the layers and build steps of the hello-cmake image
$ docker image history lcd-test

# Run the image (container removed after exit with --rm)
$ docker run --rm lcd-test

Running main() from /googletest/googletest/src/gtest_main.cc
[==========] Running 5 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 5 tests from LCDTestGroup
[ RUN      ] LCDTestGroup.ClearTest
[       OK ] LCDTestGroup.ClearTest (0 ms)
[ RUN      ] LCDTestGroup.PrintCharTest
[       OK ] LCDTestGroup.PrintCharTest (0 ms)
[ RUN      ] LCDTestGroup.PrintCharPtrTest
[       OK ] LCDTestGroup.PrintCharPtrTest (0 ms)
[ RUN      ] LCDTestGroup.PrintTest
[       OK ] LCDTestGroup.PrintTest (0 ms)
[ RUN      ] LCDTestGroup.PolymorphismTest
[       OK ] LCDTestGroup.PolymorphismTest (0 ms)
[----------] 5 tests from LCDTestGroup (0 ms total)

[----------] Global test environment tear-down
[==========] 5 tests from 1 test suite ran. (0 ms total)
[  PASSED  ] 5 tests.
```

To remove the local Docker image when it's no longer needed:

```bash
$ docker image rm lcd-test
```

## References
* [DockerHub: Docker Official Image - alpine](https://hub.docker.com/_/alpine)

* [googletest](https://github.com/google/googletest)

* Jeff Nickoloff. **Docker in Action**. Manning, 2016 

*Egon Teiniker, 2025, GPL v3.0*
