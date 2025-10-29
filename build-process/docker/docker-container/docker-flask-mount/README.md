# Example: Flask Image Using Bind Mount

When we use a bind mount, a file or directory on the host machine is mounted 
from the host into a container.

A bind mount lets us **map a folder on our host machine into a folder inside a container**:
* Files inside the mapped folder are shared
* Any change on the host appears in the container immediately
* Useful for local development because we can **edit files without rebuilding the image**

Bind mounts are used by Docker for:
* Sharing configuration files
* Letting containers read/write host data
* **Live development** (code changes without rebuilding images)


## Dockerfile

```dockerfile
# Use the official Python 3 base image
FROM python:3-alpine

# Set the working directory inside the container
WORKDIR /app  

# Inform Docker that the container listens on port 8080
EXPOSE 8080

# Copy the requirements file into the working directory
COPY requirements.txt ./  
# Install Python dependencies without caching wheels
RUN pip install --no-cache-dir -r requirements.txt  

# Default command: run the hello.py script
CMD [ "python", "./book_service.py" ]  
```


```bash
# Build the Docker image with the tag "flask-book-service"
$ docker build -t flask-book-service .

# List local images to confirm the image was created
$ docker image ls -a
REPOSITORY     TAG       IMAGE ID       CREATED          SIZE
flask-book-service   latest    707546f38c68   2 hours ago     60.7MB
```

## Start the Container with the REST Service 

```bash
# Run the image (container removed after exit with --rm)
$ docker run --rm -p 8080:8080 -v "$(pwd)/src:/app" flask-book-service 
```

* `-v "$(pwd)/src:/app"`: This is the bind mount: 
    - Use the local `src/` folder as the `/app` folder 
        so files are available inside the container. 
    - Edits to Python files on the host are reflected immediately in the 
        running container.
    - A bind mount overwrites whatever is in the container image at that path.

While the container is running, **inspect its configuration** to confirm 
the bind mount settings:

```bash
$ docker container ls -a
CONTAINER ID   IMAGE                COMMAND                  CREATED          STATUS                  PORTS                                         NAMES
72fa6bbb0f56   flask-book-service   "python ./book_servi…"   26 minutes ago   Up 26 minutes           0.0.0.0:8080->8080/tcp, [::]:8080->8080/tcp   affectionate_torvalds

$ docker container inspect 72fa6bbb0f56
    ...
    "Mounts": [
        {
            "Type": "bind",
            "Source": "/home/student/github/teiniker-lectures-embedded-cicd/build-process/docker/docker-container/docker-flask-mount/src",
            "Destination": "/app",
            "Mode": "",
            "RW": true,
            "Propagation": "rprivate"
        }
    ],
    ...
```

## Access the REST Service

We access the REST service in the Docker container using **curl**:

```bash
# Find all Books
$ curl -i http://localhost:8080/books

# Find a particular Book
$ curl -i http://localhost:8080/books/1

# Insert a Book
$ curl -i -X POST http://localhost:8080/books -H "Content-Type: application/json" -d '{"id":7, "author":"Wes McKinney ", "title":"Python for Data Analysis", "isbn":"978-1098104030"}'

# Update a Book
$ curl -i -X PUT http://localhost:8080/books/2 -H "Content-Type: application/json" -d '{"author":"Brett Slatkin","title":"Effective Python", "isbn":"0134853989"}'

# Delete a Book
$ curl -i -X DELETE http://localhost:8080/books/1
```

The `--rm` option removes the container immediately after use. 
To also remove the image, run the following command:

```bash
$ docker image rm flask-book-service
```


## References

* [Bind mounts](https://docs.docker.com/engine/storage/bind-mounts/)

* Jeff Nickoloff. **Docker in Action**. Manning, 2016 

*Egon Teiniker, 2025, GPL v3.0*
