# Example: Python BCrypt 

This example demonstrates how to build a Python image for a Python application 
using the BCrypt and Cryptography libraries.

## Dockerfile

```dockerfile
# Use the official Python 3 base image
FROM python:3-alpine

# Set the working directory inside the container
WORKDIR /app

# Copy the requirements file into the working directory
COPY requirements.txt ./
# Install Python dependencies without caching wheels
RUN pip install --no-cache-dir -r requirements.txt

# Copy the Python source code into the container's working directory
COPY ./src .
# Default command: run the password_bcrypt.py script
CMD [ "python", "./password_bcrypt.py" ]
```

For the Python application we need the following dependencies 
defined in a `requirements.txt`file:

```bash
bcrypt==5.0.0
cffi==2.0.0
cryptography==46.0.3
pycparser==2.23
```


```bash
# Build the Docker image with the tag "python-bcrypt"
$ docker build -t python-bcrypt .

# List local images to confirm the image was created
$ docker image ls -a
REPOSITORY     TAG       IMAGE ID       CREATED          SIZE
python-bcrypt   latest    fad0da3916dd   5 seconds ago   73.8MB


# Show the layers and build steps of the "python-bcrypt" image
$ docker image history python-bcrypt

# Run the image (container removed after exit with --rm)
$ docker run --rm python-bcrypt
```

To remove the local Docker image when it's no longer needed:

```bash
$ docker image rm python-bcrypt
```


## References

* [DockerHub: Docker Official Image - python](https://hub.docker.com/_/python)

* [GitHUb: bcrypt](https://github.com/pyca/bcrypt/)

* Jeff Nickoloff. **Docker in Action**. Manning, 2016 

*Egon Teiniker, 2025, GPL v3.0*
