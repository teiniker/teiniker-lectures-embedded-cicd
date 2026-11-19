# Dev Container for Python (Flask)

In this example we use the same Flask service as in the `docker-flask`
example, but this time the Docker container is used as a
**development environment** and not as a runtime environment.

## Project Structure

```
dev-container-flask/
├── .devcontainer/
│   ├── devcontainer.json
│   └── Dockerfile
├── requirements.txt
└── src/
    ├── book.py
    └── book_service.py
```

The `.devcontainer/` directory is detected automatically by VS Code.


## Dockerfile

The Dockerfile installs the tools and the Python libraries listed in
`requirements.txt`. There is no `COPY` of the source code and no `CMD`,
because VS Code mounts the project directory into the container and we
start the service ourselves.

```dockerfile
# Use the official Python image (Alpine variant) as the base image
FROM python:3-alpine

# Install git and bash for the development workflow
RUN apk add --no-cache git bash

# Install the Python libraries
# (the build context is the project directory, see devcontainer.json)
COPY requirements.txt /tmp/requirements.txt
RUN pip install --no-cache-dir --root-user-action=ignore \
        -r /tmp/requirements.txt \
    && rm /tmp/requirements.txt
```

* The Python interpreter and `pip` are already part of the base image.
* `requirements.txt` is only needed during the build, so it is copied to
    `/tmp` and removed after the installation.
* `--no-cache-dir` keeps the image small.
* `--root-user-action=ignore` suppresses the warning that `pip` is
    running as `root`.

Because the libraries are part of the image, they are available as soon
as the container starts.


## devcontainer.json

The file `devcontainer.json` describes how VS Code should create and
configure the container.

```json
{
    "name": "Python (Alpine, Flask)",
    "build": {
        "dockerfile": "Dockerfile",
        "context": ".."
    },
    "customizations": {
        "vscode": {
            "extensions": [
                "ms-python.python"
            ]
        }
    },
    "forwardPorts": [8080]
}
```

* `name`: The name shown in the VS Code status bar.
* `build.dockerfile`: The Dockerfile used to build the image (relative to
    `devcontainer.json`).
* `build.context`: The Docker build context, here the project directory.
    This is needed, because the Dockerfile copies `requirements.txt`
    from there.
* `customizations.vscode.extensions`: VS Code extensions which are installed
    **inside** the container.
* `forwardPorts`: Ports of the container which are forwarded to the host.
    Port `8080` is used by the Flask service, so it can be reached from
    the host via `http://localhost:8080`.

By default, the project directory is mounted to
`/workspaces/dev-container-flask` inside the container.


## Open the Project in a Dev Container

Requirements: Docker and VS Code with the **Dev Containers** extension.

1. Open the `dev-container-flask` directory in VS Code.
2. VS Code detects the `.devcontainer/` directory and offers
    **Reopen in Container**. Alternatively, use the command palette
    (`Ctrl+Shift+P`): **Dev Containers: Reopen in Container**.
3. The first start builds the image, including the Python libraries,
    which takes a moment. Later starts reuse the image.
4. The status bar (bottom left) shows
    **Dev Container: Python (Alpine, Flask)**.


## Run the Service Inside the Container

Open a terminal in VS Code (**Terminal > New Terminal**).
This terminal runs inside the container:

```bash
$ cat /etc/os-release | head -1
NAME="Alpine Linux"

$ python --version
Python 3.x.y

$ pip list | grep Flask
Flask        3.1.2

$ cd src
$ python book_service.py
 * Serving Flask app 'book_service'
 * Debug mode: on
 * Running on http://127.0.0.1:8080
 ...
```

Because of `forwardPorts`, the service can be tested from the host
(or from a second VS Code terminal):

```bash
$ curl -i http://localhost:8080/books/1
HTTP/1.1 200 OK
...
{
  "author": "Eric Matthes",
  "id": 1,
  "isbn": "978-1718502703",
  "title": "Python Crash Course"
}
```

The service runs in debug mode, so when we change `book_service.py` and
save the file, Flask reloads the service automatically. There is no need
to rebuild the Docker image. The image only has to be rebuilt when the
Dockerfile, `devcontainer.json` or `requirements.txt` changes
(**Dev Containers: Rebuild Container**).


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


## Back to the Host

To leave the container, use the command palette:
**Dev Containers: Reopen Folder Locally**.

The created container and image remain on the host and can be listed with:

```bash
$ docker container ls -a
$ docker image ls
```


## References
* [Development Containers](https://containers.dev/)
* [Dev Container metadata reference](https://containers.dev/implementors/json_reference/)
* [VS Code: Developing inside a Container](https://code.visualstudio.com/docs/devcontainers/containers)
* [VS Code: Python in a Container](https://code.visualstudio.com/docs/containers/quickstart-python)

*Egon Teiniker, 2025-2026, GPL v3.0*
