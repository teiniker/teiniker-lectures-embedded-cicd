# Example: Python Image 

This example demonstrates how to build a Python image for a simple Python application 
and its dependencies.

## Dockerfile

```dockerfile
# Use the official Python 3 base image
FROM python:3-alpine

# Set the working directory inside the container
WORKDIR /app  

# Copy the entire project into the container's working directory
COPY . .  
# Default command: run the hello.py script
CMD [ "python", "./hello.py" ]  
```

The `python:3-alpine` image is based on the popular Alpine Linux project.
Alpine Linux is much smaller than most distribution base images (~5MB), 
and thus leads to much slimmer images in general.
⁠

```bash
# Build the Docker image with the tag "hello-cmake"
$ docker build -t hello-python .

# List local images to confirm the image was created
$ docker image ls -a
REPOSITORY     TAG       IMAGE ID       CREATED          SIZE
hello-python   latest    9af6392bcc2c   6 seconds ago   75.7MB

# Show the layers and build steps of the hello-python image
$ docker image history hello-python

# Run the image (container removed after exit with --rm)
$ docker run --rm hello-python
Hello world using Python!
Hello world using Python!
Hello world using Python!
Hello world using Python!
Hello world using Python!
Hello world using Python!
Hello world using Python!
Hello world using Python!
Hello world using Python!
Hello world using Python!
```

The `--rm` option removes the container immediately after use. 
To also remove the image, run the following command:

```bash
$ docker image rm hello-python
```


## References

* [DockerHub: Docker Official Image - python](https://hub.docker.com/_/python)

* Jeff Nickoloff. **Docker in Action**. Manning, 2016 

*Egon Teiniker, 2025, GPL v3.0*
