# Docker 

The process of **deploying a software system** and running an application 
should be **repeatable**. 

Docker is a **virtualization technology** that significantly simplifies this 
deployment process.

The basic concept behind Docker is to **run an application, and all its 
dependencies, completely isolated**, as if only your application is 
running on a server.


* **Docker Container** 
    - Using Docker Containers    
        - [Introduction](docker-container/introduction/README.md)
        - [Creating and Starting a Container](docker-container/docker-nginx/)
        - [Running Interactive Containers](docker-container/docker-busybox/)
    
    - Building Docker Images
        - [Introduction](docker-container/docker-build/README.md)
        - [Python Docker Image](docker-container/docker-python/)
        - [C++ Docker Image](docker-container/docker-cxx/)
        - [C++ Multi-Stage Docker Image](docker-container/docker-cxx-multi-stage/)
        - [Flask Docker Image](docker-container/docker-flask/)

    - Docker Bind Mounts and Volumes 
        - [Flask Bind Mount Image](docker-container/docker-flask-mount/)       
        - [MSQL Server](docker-container/docker-mysql/)

    - [Docker Security Aspects](docker-container/docker-security/README.md)


* **Docker Compose**
    - [Introduction](docker-compose/introduction/README.md)

    - Creating and Starting Services
        - [Example: Flask + Redis](docker-compose/compose-flask/)


*Egon Teiniker, 2025, GPL v3.0*