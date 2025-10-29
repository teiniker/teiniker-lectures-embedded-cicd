# Building Docker Images

## Image Layers

**Container images are composed of layers**. And each of these layers, once created, 
are immutable. 

Layers let us **extend images** of others by reusing their base layers, allowing us to 
add only the data that your application needs.

This is beneficial because: 
* it allows layers to be **reused between images**. 
* it makes **builds faster** 
* it **reduces the amount of storage and bandwidth** required to distribute the images.


_Example:_ TODO: Python App1 App2 

We wanted to create another Python application. Due to layering, we can leverage 
the same Python base. 


Layering is made possible by **content-addressable storage** and **union filesystems**:

* After each layer is downloaded, it is extracted into its own directory on the host filesystem.
* When we run a container from an image, a union filesystem is created where layers are stacked 
    on top of each other, creating a new and unified view.
* When the container starts, its root directory is set to the location of this unified directory, 
    using `chroot`.

When the union filesystem is created, in addition to the image layers, 
a **directory is created specifically for the running container**. 
This allows the container to make **filesystem changes** while allowing the 
original image layers to remain untouched. This enables you to run multiple 
containers from the same underlying image.


## Base Image 

A **base image** is the starting layer of a Docker image, the foundation on top 
of which all other layers are added. It’s the first image you specify in a 
`Dockerfile` using the `FROM` instruction.

```dockerfile
FROM alpine:latest
#...
```

The `alpine` base image is a minimal Docker image based on **Alpine Linux** 
with a complete package index and only **5 MB** in size.

The `alpine` image uses the **Alpine Package Keeper (apk)** package manager.
apk is lightweight, fast, and designed specifically for Alpine’s minimal 
footprint. It’s used to install, update, and remove software packages inside 
Alpine-based containers.

* **Install a package**: `apk add g++`
* **Remove a package**: `apk del g++`
* **Update package index**: `apk update`
* **Upgrade installed packages**: `apk upgrade`

Alpine Linux uses the **Alpine Package** format `.apk`.
Each `.apk` file is a compressed tar archive (tar.gz or tar.xz)

We can look up apk packages in the official 
[**Alpine Linux package repositories**](https://pkgs.alpinelinux.org/packages)




## References

* [Understanding the image layers](https://docs.docker.com/get-started/docker-concepts/building-images/understanding-image-layers/)

* [DockerHub: Docker Official Image - alpine](https://hub.docker.com/_/alpine)


*Egon Teiniker, 2025, GPL v3.0*
