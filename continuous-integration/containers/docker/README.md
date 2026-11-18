# Docker 

Docker is a **virtualization technology** that significantly simplifies this 
deployment process.

* **Docker Container** 
    - Using Docker Containers    
        - [Introduction](docker-container/introduction/README.md)
        - Demo: [docker-nginx](docker-container/docker-nginx/)
        - Demo: [docker-busybox](docker-container/docker-busybox/)
    
    - Building Docker Images
        - [Introduction](docker-container/docker-build/README.md)
        - Demo: [docker-python](docker-container/docker-python/)
        - Demo: [docker-flask](docker-container/docker-flask/)
        - Demo: [docker-cxx](docker-container/docker-cxx/)
        - Demo: [docker-cxx-multi-stage](docker-container/docker-cxx-multi-stage/)

    - Docker Bind Mounts  
        - Demo: [docker-flask-mount](docker-container/docker-flask-mount/)       

    - Docker Volumes 
        - [Introduction ](docker-container/docker-volumes/)
        - Demo: [docker-mariadb](docker-container/docker-mariadb)


* **Docker Compose**
    - [Introduction](docker-compose/introduction/README.md)

    - Creating and Starting Services
        - Demo: [MariaDB + mysql Client](docker-compose/compose-mariadb)
        - Demo: [Flask + Redis](docker-compose/compose-flask/)

* **Docker on Raspberry Pi 5**
    - [Setup Docker on Raspi5](docker-raspi5/README.md)

* **Multi-Architecture Images**
    - [Introduction](docker-container/docker-buildx/README.md)
    - Demo: [C++ Docker Image for ARM64](docker-container/docker-cxx-arm64/)
    - Demo: [C++ Multi-Stage Docker Image for ARM64](docker-container/docker-cxx-multi-stage-arm64/)

*Egon Teiniker, 2025-2026, GPL v3.0*
