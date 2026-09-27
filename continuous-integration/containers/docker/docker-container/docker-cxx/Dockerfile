# Use Alpine as the base image
FROM alpine:latest

# Install required packages: g++, make, cmake, and build tools
RUN apk add --no-cache g++ make cmake

# Set the working directory
WORKDIR /app

# Copy project files into the container
COPY . .

# Create a build directory and compile using CMake
RUN mkdir -p build && cd build && cmake .. && make

# Set the default working directory to build output
WORKDIR /app/build

# Default command to run the compiled program (assuming target is 'app')
CMD ["./hello"]
