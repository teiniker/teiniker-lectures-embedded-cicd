# Dev Container for C++

In this example we use the same C++ program and CMake build as in the
`docker-cxx` example, but this time the Docker container is used as a
**development environment** and not as a runtime environment.

## Project Structure

```
dev-container-cxx/
├── .devcontainer/
│   ├── devcontainer.json
│   └── Dockerfile
├── CMakeLists.txt
└── hello.cpp
```

The `.devcontainer/` directory is detected automatically by VS Code.


## Dockerfile

The Dockerfile only installs the tools. There is no `COPY` and no build
step, because VS Code mounts the project directory into the container.

```dockerfile
# Use Alpine as the base image
FROM alpine:latest

# Install the C++ toolchain, build tools, debugger and git
RUN apk add --no-cache g++ make cmake gdb git bash
```


## devcontainer.json

The file `devcontainer.json` describes how VS Code should create and
configure the container.

```json
{
    "name": "C++ (Alpine, CMake)",
    "build": {
        "dockerfile": "Dockerfile",
        "context": ".."
    },
    "customizations": {
        "vscode": {
            "extensions": [
                "ms-vscode.cpptools",
                "ms-vscode.cmake-tools"
            ]
        }
    },
    "postCreateCommand": "cmake -S . -B build"
}
```

* `name`: The name shown in the VS Code status bar.
* `build.dockerfile`: The Dockerfile used to build the image (relative to
    `devcontainer.json`).
* `build.context`: The Docker build context, here the project directory.
* `customizations.vscode.extensions`: VS Code extensions which are installed
    **inside** the container.
* `postCreateCommand`: A command which is executed once after the container
    has been created. Here we configure the CMake build.

By default, the project directory is mounted to
`/workspaces/dev-container-cxx` inside the container.


## Open the Project in a Dev Container

Requirements: Docker and VS Code with the **Dev Containers** extension.

1. Open the `dev-container-cxx` directory in VS Code.
2. VS Code detects the `.devcontainer/` directory and offers
    **Reopen in Container**. Alternatively, use the command palette
    (`Ctrl+Shift+P`): **Dev Containers: Reopen in Container**.
3. The first start builds the image, which takes a moment. Later starts
    reuse the image.
4. The status bar (bottom left) shows
    **Dev Container: C++ (Alpine, CMake)**.


## Build and Run Inside the Container

Open a terminal in VS Code (**Terminal > New Terminal**).
This terminal runs inside the container:

```bash
$ cat /etc/os-release | head -1
NAME="Alpine Linux"

$ g++ --version
g++ (Alpine 15.2.0) 15.2.0

$ cmake -S . -B build
$ cmake --build build
$ ./build/hello
Hello world using CMake!
Hello world using CMake!
...
```

The `build/` directory is created in the mounted project directory, so it
is also visible on the host.

When we change `hello.cpp`, we just build again. There is no need to
rebuild the Docker image. The image only has to be rebuilt when the
Dockerfile or `devcontainer.json` changes
(**Dev Containers: Rebuild Container**).


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

*Egon Teiniker, 2025-2026, GPL v3.0*
