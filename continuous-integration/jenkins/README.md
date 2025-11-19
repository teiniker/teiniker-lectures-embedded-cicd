# Jenkins 

> Jenkins is an **open-source automation server** used mainly for 
> **continuous integration (CI)** and **continuous delivery (CD)**.


## Jenkins Pipeline

Jenkins uses a `Jenkinsfile` which is part of a project to configure the 
steps needed in the CI/CD pipeleine.



## Controller and Agents 


## Setup 

There a many ways to setup a Jenkins server - it depends on our use case.

### WAR File Setup 

A **WAR (Web Application Archive)** is a Java web application packaged for 
deployment. The Jenkins WAR can be run directly with a compatible Java 
runtime (for example, JDK 21) using `java -jar jenkins.war`, so we can 
start Jenkins without installing a separate application server.

* Download Jenkins Generic Java package (.war): [https://www.jenkins.io/download/](https://www.jenkins.io/download/) 

* In the download directory
    ```bash
    $ java -jar jenkins.war --httpPort=8080
    ```

* Browse to http://localhost:8080

* Follow the instructions to complete the installation


### Docker Setup 



## References

* [Jenkins Homepage](https://www.jenkins.io/)


_Egon Teiniker, 2025, GPL v3.0_