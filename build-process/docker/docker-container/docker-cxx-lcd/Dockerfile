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
