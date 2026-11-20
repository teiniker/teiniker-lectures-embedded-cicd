# Example: Flask Image Using Bind Mount

In this example we use a **bind mount** for live development of a Flask
REST service: the source code stays on the host, where we edit it, and
runs inside a container.

## Project Structure

```
docker-flask-mount/
├── Dockerfile
├── requirements.txt
└── src/
    ├── book.py
    └── book_service.py
```


## Dockerfile

```dockerfile
FROM python:3-alpine

WORKDIR /app

COPY requirements.txt ./
RUN pip install --no-cache-dir -r requirements.txt

EXPOSE 8080

CMD [ "python", "./book_service.py" ]
```

* `FROM python:3-alpine`: Use the official Python 3 base image (Alpine
    variant).
* `WORKDIR /app`: Set the working directory inside the container.
* `COPY requirements.txt` and `RUN pip install`: Install the Python
    libraries (Flask) into the image. `--no-cache-dir` keeps the image
    small.
* `EXPOSE 8080`: Document that the service listens on port 8080.
* `CMD`: Start the REST service `book_service.py`.

Note that there is **no `COPY` for the source code**. The image only
contains the Python interpreter and the libraries. The code will be
provided by the bind mount.

```bash
# Build the Docker image with the tag "flask-book-service"
$ docker build -t flask-book-service .

# List local images to confirm the image was created
$ docker image ls flask-book-service
IMAGE                       ID             DISK USAGE   CONTENT SIZE   EXTRA
flask-book-service:latest   2c09db68c898        106MB         26.2MB
```

If we start the image **without** a bind mount, the container stops
immediately, because `/app` doesn't contain the service:

```bash
$ docker run --rm flask-book-service
python: can't open file '/app/./book_service.py': [Errno 2] No such file or directory
```


## Start the Container with the REST Service

```bash
# Run the image (container removed after exit with --rm)
$ docker run --rm -p 8080:8080 -v "$(pwd)/src:/app" flask-book-service
```

* `-v "$(pwd)/src:/app"`: This is the bind mount:
    - The local `src/` directory appears as the `/app` directory in the
        container, so `book_service.py` is found.
    - The bind mount hides whatever the image contains at `/app`.
* `-p 8080:8080`: Publish port 8080 of the container on the host.
* `--rm`: Remove the container when it stops.

While the container is running, we **inspect its configuration** in a
second terminal to confirm the bind mount settings:

```bash
$ docker container ls
CONTAINER ID   IMAGE                COMMAND                  CREATED         STATUS         PORTS                                         NAMES
72fa6bbb0f56   flask-book-service   "python ./book_servi…"   2 minutes ago   Up 2 minutes   0.0.0.0:8080->8080/tcp, [::]:8080->8080/tcp   affectionate_torvalds

$ docker container inspect 72fa6bbb0f56
    ...
    "Mounts": [
        {
            "Type": "bind",
            "Source": "/home/student/github/teiniker-lectures-embedded-cicd/continuous-integration/containers/docker/docker-container/docker-flask-mount/src",
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
$ curl -i -X POST http://localhost:8080/books -H "Content-Type: application/json" -d '{"id":7, "author":"Wes McKinney", "title":"Python for Data Analysis", "isbn":"978-1098104030"}'

# Update a Book
$ curl -i -X PUT http://localhost:8080/books/2 -H "Content-Type: application/json" -d '{"author":"Brett Slatkin","title":"Effective Python", "isbn":"0134853989"}'

# Delete a Book
$ curl -i -X DELETE http://localhost:8080/books/1
```


## Live Development

Now we change the source code **on the host**, for example the error
message in `src/book_service.py`:

```python
return jsonify({'message': 'book not found (edited)'}), HTTPStatus.NOT_FOUND
```

After saving the file, the container log shows that the service was
reloaded:

```
 * Detected change in '/app/book_service.py', reloading
 * Restarting with stat
```

And the service immediately uses the new code:

```bash
$ curl http://localhost:8080/books/999
{
  "message": "book not found (edited)"
}
```

Two things work together here:
* The **bind mount** makes the changed file visible in the container.
* The **Flask debug mode** (`app.run(..., debug=True)` in
    `book_service.py`) watches the source files and restarts the service
    when a file changes.

Without the debug mode, the file in the container would also change, but
the running Python process would still use the old code until the
container is restarted. In both cases, there is **no need to rebuild the
image**. A rebuild is only necessary when `Dockerfile` or
`requirements.txt` changes.

Note that the in-memory book table is reset with every reload, because
the service is restarted.


## Clean Up

We stop the service with `Ctrl+C` in the terminal where it runs. The
`--rm` option removes the container immediately. To also remove the
image, run the following command:

```bash
$ docker image rm flask-book-service
```


## References

* [Docker Bind Mounts](../docker-mount/)
* [Docker: Bind mounts](https://docs.docker.com/engine/storage/bind-mounts/)
* [Flask: Debug Mode](https://flask.palletsprojects.com/en/stable/quickstart/#debug-mode)
* Jeff Nickoloff. **Docker in Action**. Manning, 2016

*Egon Teiniker, 2025, GPL v3.0*
