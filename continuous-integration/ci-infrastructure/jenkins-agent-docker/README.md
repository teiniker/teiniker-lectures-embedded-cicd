# Example: Jenkins and Docker Agent

## Setup Jenkins

Start the native Jenkins service:

```bash
$ sudo systemctl start jenkins
$ sudo systemctl status jenkins	
```

Access the Jenkins Web GUI:

```bash
URL: http://localhost:8080/
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
      - Pipeleine: Pipeline Script from SCM: 
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
	    - Poll SCM: `H/1 * * * *`
    - Pipeline / Definition: 
      - Pipeleine: Pipeline Script from SCM: 
        - SCM: Git 
        - Repository URL: `https://github.com/teiniker/jenkins-pipeline-python.git` 
        - Branches to build: `*/main` 
      - Script Path: **Jenkinsfile-agent-docker**


_Egon Teiniker, 2025, GPL v3.0_    