# Multi-Architecture Images

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


## References

* [Docker - Multi-platform builds](https://docs.docker.com/build/building/multi-platform/)

*Egon Teiniker, 2025, GPL v3.0*
