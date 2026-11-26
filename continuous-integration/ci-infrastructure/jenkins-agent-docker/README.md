# Example: Jenkins and Docker Agent

## Setup Jenkins

Start the native Jenkins service:

```bash
$ sudo systemctl start jenkins
$ sudo systemctl status jenkins	
```

Access the Jenkins Web GUI:

```bash
URL: http://localhost:9000/
```


## Configure Jenkins Job (C++ Pipeline) 

We use the `jenkins-pipeline-cxx` with `Jenkinsfile-agent-docker`:

* New Item
    - Enter an item Name "**C++ Pipeline (Docker Agent)**"
	- Pipeline
    - Description (optional)
    - Build Triggers 
	    - Poll SCM: `H/1 * * * *`
    - Pipeline / Definition: 
      - Pipeline: Pipeline Script from SCM: 
        - SCM: Git 
        - Repository URL: `https://github.com/teiniker/jenkins-pipeline-cxx.git` 
        - Branches to build: `*/main` 
      - Script Path: **Jenkinsfile-agent-docker**

Note that the `Jenkinsfile-agent-docker` and the `Dockerfile` are located in 
the project repository and can therefore be modified by the developers at 
any time.


## Jenkins Job Configuration (Python Pipeline)

We need the following settings for a new Jenkins job:

* New Item 
    - Enter an item Name "**Python Pipeline (Docker Agent)**"
	- Pipeline
    - Description (optional)
    - Build Triggers 
	    - Poll SCM: `H/2 * * * *`
    - Pipeline / Definition: 
      - Pipeline: Pipeline Script from SCM: 
        - SCM: Git 
        - Repository URL: `https://github.com/teiniker/jenkins-pipeline-python.git` 
        - Branches to build: `*/main` 
      - Script Path: **Jenkinsfile-agent-docker**


## Any vs. Docker

The `jenkins-pipeline-cxx` repository contains two pipelines which only
differ in the `agent` directive:

```groovy
// Jenkinsfile
pipeline
{
    agent any
    ...
}
```

```groovy
// Jenkinsfile-agent-docker
pipeline
{
    agent { dockerfile true }
    ...
}
```

* `agent any`: The build steps run **directly on a node** (in our setup,
    the built-in node of the Jenkins controller). They use the tools
    which are **installed on that machine**.

* `agent { dockerfile true }`: Jenkins builds a Docker image from the
    `Dockerfile` in the root directory of the repository, starts a
    container from this image, and runs the build steps **inside the
    container**. They use the tools which are **installed in the image**.

### The Build Environment Comes from the Repository

The `Dockerfile` of `jenkins-pipeline-cxx` describes everything the build
needs:

```dockerfile
FROM alpine:latest

# Install required packages: g++, make, cmake, and build tools
RUN apk add --no-cache git g++ make cmake

# Install Google Test framework
RUN git clone -q https://github.com/google/googletest.git /googletest \
  && mkdir -p /googletest/build \
  && cd /googletest/build \
  && cmake .. && make && make install \
  && cd / && rm -rf /googletest

WORKDIR /app
```

So the two pipelines use **different compilers**, although they run on
the same machine:

```bash
# Installed on the Jenkins machine (used by "agent any")
$ g++ --version
g++ (Debian 14.2.0-19) 14.2.0
$ cmake --version
cmake version 3.31.6

# Installed in the Docker image (used by "agent { dockerfile true }")
$ docker run --rm <image> sh -c 'g++ --version; cmake --version; ls /usr/local/lib'
g++ (Alpine 15.2.0) 15.2.0
cmake version 4.2.3
libgtest.a
libgtest_main.a
...
```

With `agent any`, an administrator has to install g++, CMake and Google
Test on every node, in the versions the project needs. If two projects
need different versions, they get in each other's way. With a
`Dockerfile`, the **developers** define the build environment, and it is
versioned together with the code. A change of the compiler is a commit,
like any other change.

### What Jenkins Does with `dockerfile true`

The console output of the build shows the individual steps:

```
Running on Jenkins in /var/lib/jenkins/workspace/C++ Pipeline (Docker Agent)
...
[Pipeline] { (Declarative: Agent Setup)
+ docker build -t b8b555c81c1170caea5ebd25ec477830b75a76e3 -f Dockerfile .
...
$ docker run -t -d -u 112:117 -w "/var/lib/jenkins/workspace/C++ Pipeline (Docker Agent)"
    -v "/var/lib/jenkins/workspace/C++ Pipeline (Docker Agent):/var/lib/jenkins/workspace/C++ Pipeline (Docker Agent):rw,z"
    ... b8b555c81c1170caea5ebd25ec477830b75a76e3 cat
...
+ mkdir -p build
+ cd build
+ cmake ..
+ make
+ ./test/test
[  PASSED  ] 2 tests.
Archive build artifacts
$ docker stop --time=1 a62fce2b59b2...
$ docker rm -f --volumes a62fce2b59b2...
Finished: SUCCESS
```

1. **Select a node:** The Docker agent still needs a node on which
    Docker runs. Here it is the built-in node (`Running on Jenkins`).
    The repository is checked out into the workspace of this node.

2. **Build the image:** `docker build` with the `Dockerfile` from the
    workspace. Jenkins generates the image name (a hash). Thanks to the
    Docker layer cache, the next builds reuse the existing layers and
    only rebuild the image when the `Dockerfile` changes.

3. **Start a container:**
    - `-v ...:rw,z`: The workspace is mounted into the container as a
        **bind mount**, at the same path as on the host (see
        [Docker Bind Mounts](../../containers/docker/docker-container/docker-mount/)).
        So the build results are written to the workspace on the host.
    - `-w ...`: The working directory is the workspace.
    - `-u 112:117`: The container runs with the UID and GID of the user
        `jenkins`, so the created files belong to `jenkins` on the host.
    - `-t -d ... cat`: The container runs in the background, and `cat`
        keeps it alive. It does nothing on its own.

4. **Execute the steps:** Every `sh` step is executed **inside the
    container** (like with `docker exec`).

5. **Clean up:** After the last stage, Jenkins stops and removes the
    container. The image remains on the node.

Because the workspace is a bind mount, steps which run outside the
container, like `archiveArtifacts`, find the build results
(`file_service/build/test/test`) in the workspace.


### Requirements and Security

The Docker agent requires:
* The **Docker Pipeline** plugin in Jenkins,

* Docker on the node, and permission for the Jenkins user to use it.
    On our VM, the user `jenkins` is a member of the `docker` group:

    ```bash
    $ id jenkins
    uid=112(jenkins) gid=117(jenkins) groups=117(jenkins),986(docker)
    ```

Note that membership in the `docker` group is **equivalent to root
access** on the host: whoever can start containers can mount any
directory of the host into a container. A pipeline can therefore do
everything on the node. A container isolates the **build environment**,
but it is not a security boundary against a malicious `Jenkinsfile`.


### Variants

`dockerfile true` is the short form. With a block, we can configure the
image build and choose the node:

```groovy
agent {
    dockerfile {
        filename 'Dockerfile.build'   // another file name
        dir 'ci'                      // Dockerfile in a subdirectory
        label 'linux-vm'              // node on which Docker runs
    }
}
```

If we don't need our own image, we use a ready-made image from a
registry with the `docker` agent:

```groovy
agent {
    docker { image 'gcc:15' }
}
```


_Egon Teiniker, 2025-2026, GPL v3.0_    