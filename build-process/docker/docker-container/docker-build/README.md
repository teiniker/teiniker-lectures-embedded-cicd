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








## References 

* [Understanding the image layers](https://docs.docker.com/get-started/docker-concepts/building-images/understanding-image-layers/)
