# Use Alpine as the base image
FROM alpine:latest

# Install the C++ toolchain, build tools, debugger and git
RUN apk add --no-cache g++ make cmake gdb git bash

# Note: No COPY and no build step here.
# The source code is mounted into the container by VS Code.
