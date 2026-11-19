# Docker 

Docker is a **virtualization technology** that significantly simplifies this 
deployment process.

* **Docker Container** 
    - Using Containers    
        - [Introduction](docker-container/introduction/README.md)
        - Demo: [docker-nginx](docker-container/docker-nginx/)
        - Demo: [docker-busybox](docker-container/docker-busybox/)
        - Demo: [docker-mosquitto](docker-container/docker-mosquitto/)
    - Building Images
        - [Introduction](docker-container/docker-build/README.md)
        - Demo: [docker-python](docker-container/docker-python/)
        - Demo: [docker-flask](docker-container/docker-flask/)
        - Demo: [docker-cxx](docker-container/docker-cxx/)
        - Demo: [docker-cxx-multi-stage](docker-container/docker-cxx-multi-stage/)
    - [Port Mapping](docker-container/docker-port-mapping/README.md) 
    - [Environment Variables](docker-container/docker-environment/README.md)
    - Bind Mounts  
        - Demo: [docker-flask-mount](docker-container/docker-flask-mount/)       
    - [Volumes](docker-container/docker-volumes/)


* **Docker Compose**
    - [Introduction](docker-compose/introduction/README.md)
    - [Volumes](docker-compose/compose-volumes/)
    - [Network](docker-compose/componse-network/)
    - Examples + Exercises:
        - Demo: [compose-mariadb](docker-compose/compose-mariadb)
        - Demo: [compose-message-service](docker-compose/compose-message-service/)
        - Exercise: [](docker-compose/compose-mosquitto-exercise/) ([Model Solution](docker-compose/compose-mosquitto/))


* **Multi-Architecture Images**
    - [Introduction](docker-multi-architecture/docker-buildx/README.md)
    - Demo: [C++ Docker Image for ARM64](docker-multi-architecture/docker-cxx-arm64/)
    - Demo: [C++ Multi-Stage Docker Image for ARM64](docker-multi-architecture/docker-cxx-multi-stage-arm64/)

* **Dev-Containers**
    - [Introduction](dev-containers/introduction/README.md)
    - Demo: [dev-container-cxx](dev-containers/dev-container-cxx/)
    - Demo: [dev-container-flask](dev-containers/dev-container-flask/)

*Egon Teiniker, 2025-2026, GPL v3.0*
