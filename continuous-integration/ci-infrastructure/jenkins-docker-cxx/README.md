# Example: Jenkins in Docker Container for C++ Build

### Dockerfile 

The Dockerfile extends the official `jenkins/jenkins:lts` image 
and installs plugins and development tools for C++.

```dockerfile
FROM jenkins/jenkins:lts

RUN jenkins-plugin-cli --plugins workflow-aggregator git warnings-ng

USER root

# Install required packages: g++, make, cmake, and build tools
RUN apt update && apt install -y git g++ make cmake

# Install Google Test framework
RUN git clone -q https://github.com/google/googletest.git /googletest \
  && mkdir -p /googletest/build \
  && cd /googletest/build \
  && cmake .. && make && make install \
  && cd / && rm -rf /googletest

USER jenkins
```

## Start Jenkins and Configure a Job

### Start Jenkins

Start the Jenkins container woth docker compose:

```bash
$ docker-compose up -d 
```

If we make changes to the Dockerfile (e.g., install additional development tools), 
then we need to **rebuild the image**:

```bash
$ docker-compose build
```

**Access the Jenkins Web GUI**: 

```
URL: http://localhost:8080/
```


### Jenkins Job Configuration

_Example:_ [jenkins-pipeline-cxx](https://github.com/teiniker/jenkins-pipeline-cxx)

We need the following settings for a new Jenkins job to build the 
`jenkins-pipeline-cxx` project:

* New Item 
	- Pipeline
    - Description (optional)
    - Build Triggers 
	    - Poll SCM: `H/1 * * * *`
    - Pipeline / Definition: 
      - Pipeleine: Pipeline Script from SCM: 
        - SCM: Git 
        - Repository URL: `https://github.com/teiniker/jenkins-pipeline-cxx.git` 
        - Branches to build: `*/main` 
      - Script Path: Jenkinsfile

Save these settings.


_Egon Teiniker, 2025, GPL v3.0_