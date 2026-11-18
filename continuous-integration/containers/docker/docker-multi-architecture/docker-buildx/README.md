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


## Docker Buildx
Docker Buildx is an extended build tool for Docker that adds powerful 
features on top of the standard docker build command.

We can build Docker images for multiple CPU platforms from one machine, 
such as: linux/amd64, linux/arm64, linux/arm/v7	

```bash
$ docker buildx create --name multiarch --use

# verify it’s the current builder
$ docker buildx ls         
```

* `docker buildx create`: Creates a new Buildx builder instance.
    - Buildx uses “builder instances” to control how and where images are built
    - A builder can be local, remote, multi-node, or connect to Docker 
        drivers like docker-container or kubernetes
    - We can have multiple builders and switch between them.

* `--name multiarch`: This assigns a name to the builder instance.
    - Without a name, Docker would auto-generate one.
    - Naming it helps us list, inspect, remove, or reuse it later:
        - `docker buildx rm multiarch`
        - `docker buildx use multiarch`

* `--use`: Immediately activates this builder and sets it as the current 
    default for future docker buildx build commands.
    Without `--use`, the builder is created but not selected.

Because the default Docker builder doesn’t support multi-architecture builds on its own.
Creating a new builder like this:
* Enables BuildKit (faster builds, caching, parallelism)
* Allows cross-platform builds (`--platform linux/arm64,linux/amd64`)
* Works with QEMU emulation for foreign architectures

* `docker buildx ls`: Docker prints a table showing:
    - what builders exist
    - which builder is currently active
    - what architectures they support
    - their status (running/stopped)
    - which driver each uses



### QEMU 

Next, we have to **install QEMU emulation support** on our machine so we can run 
ARM64 binaries (or build ARM64 Docker images) on a non-ARM64 host like x86-64.

This makes Docker able to execute ARM code transparently, which is crucial for 
docker buildx multi-architecture builds.

```bash
$ docker run --privileged --rm tonistiigi/binfmt --install arm64
```

* `--privileged`: Gives the container elevated permissions so it can modify 
    system settings. Required because it needs to add entries to `binfmt_misc` 
    (a kernel feature).

* `--rm`: Automatically deletes the container after it finishes.
    The changes persist, but the container isn’t left behind.

* `tonistiigi/binfmt`: This is the container image.
    It’s a lightweight tool created by a Docker BuildKit maintainer.
    Purpose: install CPU architecture interpreters for cross-execution.

* `--install arm64`: Installs the emulator for ARM64 only.
    - We could also do: `--install all` to install interpreters for most 
    architectures (`arm`, `arm64`, `s390x`, `ppc64le`, `riscv`, etc.).

Now, we can run ARM64 binaries directly:
```bash
$ ./some-arm64-binary
```

We can test ARM64 container images locally:
```bash
$ docker run --platform=linux/arm64 arm64v8/alpine 
```

We can **build ARM64 Docker images** using Buildx:

```bash
$ docker buildx build --platform linux/arm64 -t hello-cmake:raspi5 --load .
```

* `docker buildx build`: Uses Buildx, Docker’s advanced builder (BuildKit-powered).
    Supports multi-architecture builds, caching, and parallel builds.

* `--platform linux/arm64`: Tells Buildx to produce an image whose CPU/OS target is:
    - OS: Linux
    - Architecture: ARM64 (aka aarch64)

    This is what Raspberry Pi 5 uses when running a 64-bit OS.

* `-t hello-cmake:raspi5`: Tags the resulting image so Docker can refer to it by name:
    - repository: hello-cmake
    - tag: raspi5

* `--load`: Converts the built image into a format the regular Docker daemon can 
    use and loads it into the local image store. 
    - Without it, Buildx would not load the image into our local Docker
    - Buildx normally stores images in its own cache or pushes to a registry




## References

* [Docker - Multi-platform builds](https://docs.docker.com/build/building/multi-platform/)

*Egon Teiniker, 2025, GPL v3.0*
