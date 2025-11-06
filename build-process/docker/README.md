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
        - [Flask Docker Image](docker-container/docker-flask/)
        - [C++ Docker Image](docker-container/docker-cxx/)
        - [C++ Multi-Stage Docker Image](docker-container/docker-cxx-multi-stage/)

    - Docker Bind Mounts  
        - [Flask Bind Mount Image](docker-container/docker-flask-mount/)       

    - Docker Volumes 
        - [Introduction ](docker-container/docker-volumes/)
        - [MariaDB Server](docker-container/docker-mariadb)

    - Docker Security
        - [Introduction](docker-container/docker-security/README.md)


* **Docker Compose**
    - [Introduction](docker-compose/introduction/README.md)

    - Creating and Starting Services
        - [Example: MariaDB + mysql Client](docker-compose/compose-mariadb)
        - [Example: Flask + Redis](docker-compose/compose-flask/)

* **Docker on Raspberry Pi 5**
    - [Setup Docker on Raspi5](docker-raspi5/README.md)

* **Multi-Architecture Images**
    - [Introduction](docker-container/docker-buildx/README.md)
    - [C++ Docker Image for ARM64](docker-container/docker-cxx-arm64/)
    - [C++ Multi-Stage Docker Image for ARM64](docker-container/docker-cxx-multi-stage-arm64/)

*Egon Teiniker, 2025, GPL v3.0*
