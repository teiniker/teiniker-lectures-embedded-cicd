# Jenkins with Agent Setup


## docker-compose.yml File

The `docker-compose.yml` builds the Dockerfile in the current directory
into a Docker image.



### Dockerfile 

The Dockerfile extends the official `jenkins/jenkins:lts` image 
and installs plugins and development tools.




## Jenkins Configuration as Code (JCasC)

Jenkins is preconfigured by a `jenkins.yaml` file.

```bash 
jenkins:
  systemMessage: "Jenkins automatically pre-configured."

  securityRealm:
    local:
      allowsSignup: false
      users:
        - id: "admin"
          password: "admin123"

  authorizationStrategy:
    loggedInUsersCanDoAnything:
      allowAnonymousRead: false

unclassified:
  location:
    url: "http://localhost:8080/"
```

* Security Realm: 
  - Creates a local admin account automatically.

* Authorization Strategy:
  - Logged-in users have full access.
  - Anonymous users cannot read anything.

* Location:
  - Defines the Jenkins base URL.


## Start Jenkins and Configure a Job


### Start Jenkins

With the help of Docker Compose, it is very easy to start the Jenkins 
container:

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

We need the following settings for a new Jenkins job:

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



## References 

- [Github: Jenkins Configuration as Code (a.k.a. JCasC) Plugin](https://github.com/jenkinsci/configuration-as-code-plugin)
- [Jenkins: Configuration as Code](https://www.jenkins.io/projects/jcasc/)
- [Jenkins: Managing Plugins](https://www.jenkins.io/doc/book/managing/plugins/#installing-plugins)


_Nicoletta Kähling, Egon Teiniker, 2025, GPL v3.0_