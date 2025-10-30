# Introduction to Docker Compose

Docker Compose is a tool for **defining and running multi-container applications**. 

It is the key to unlocking a streamlined and efficient development and deployment experience.

Compose simplifies the control of your entire application stack, making it easy to manage 
services, networks, and volumes in a single YAML configuration file. Then, with a single 
command, you create and start all the services from your configuration file.

Compose works in all environments - production, staging, development, testing, as well 
as CI workflows.

Docker Compose shifts the focus from individual containers to defining **complete environments** 
and the interactions between **service components**.

With Compose we can use simple commands to accomplish these tasks:
* Build Docker images
* Launch containerized applications as services
* Launch full systems of services
* Manage the state of individual services in a system
* Scale services up or down
* View logs for the collection of containers making a service


From within the **directory which contains the docker-compose-yml file**, we can manage 
this system of containers with the following commands:
```
$ docker compose up         Create and start containers

$ docker compose ps         List containers

$ docker compose stop       Stop services
$ docker compose start      Start services

$ docker compose rm         Remove stopped containers

$ docker compose down       Stop all containers and remove them
```

## YAML Configuration 
Docker Compose works by applying many rules declared within a single `docker-compose.yml` 
configuration file.

In Docker Compose, **Services** are the building blocks and refer to Docker container 
configurations. 

A `docker-compose.yml` file usually defines many services:
```
services:
  frontend:
    image: my-vue-app
    ...
  backend:
    image: my-springboot-app
    ...
  db:
    image: postgres
    ...
``` 
There are multiple settings that we can apply to services.

### Configuring Images
Sometimes, the **image** we need for our service has already been published 
(by us or by others) in Docker Hub, or another Docker Registry.
If that's the case, then we can **pull an image** using the `image` attribute, 
by specifying the image name and tag:
```
services: 
  my-service:
    image: ubuntu:latest
    ...
```

Instead, we might need to **build an image** from the source code by reading 
its `Dockerfile`.
This time, we'll use the `build` keyword, passing the path to the Dockerfile 
as the value:

```
services: 
  my-custom-app:
    build: /path/to/dockerfile/
    ...
```

Note that we can also use a URL instead of a path.
Additionally, we can specify an image name in conjunction with the `build` attribute, 
which will name the image once created: 
```
services: 
  my-custom-app:
    build: https://github.com/my-company/my-project.git
    image: my-project-image
    ...
```


### Configuring Environment Variables
We can define **static environment variables**, and also define **dynamic variables** 
with the `${}` notation:
```
services:
  database: 
    image: "postgres:${POSTGRES_VERSION}"
    environment:
      DB: mydb
      USER: "${USER}"
```
There are different methods to provide those values to Compose.
* We can  setting them in a **.env file** in the same directory, structured like 
  a `.properties` file, `key=value`

    ```
    POSTGRES_VERSION=alpine
    USER=foo
    ```
* We can **set them in the OS** before calling the command:
  
    ```
    export POSTGRES_VERSION=alpine
    export USER=foo
    docker compose up
    ```
  
    We might find handy using a simple one-liner in the shell:
  
    ```
    POSTGRES_VERSION=alpine USER=foo docker compose up
    ```

We can mix the approaches, but let's keep in mind that Compose uses the following 
**priority order**, overwriting the less important with the higher ones:
1. Compose file
2. Shell environment variables
3. Environment file
4. Dockerfile
5. Variable not defined


### Configuring Dependencies
Often, we need to create a **dependency chain between our services**, so that 
some services get loaded before (and unloaded after) other ones. We can achieve 
this result through the `depends_on` keyword:

```
services:
  kafka:
    image: wurstmeister/kafka:2.11-0.11.0.3
    depends_on:
      - zookeeper
    ...
  zookeeper:
    image: wurstmeister/zookeeper
    ...
```
Note that Compose will not wait for the `zookeeper` service to finish loading before 
starting the `kafka` service: it will simply wait for it to start. 



## References
* [Overview of Docker Compose](https://docs.docker.com/compose/)

* [Introduction to Docker Compose](https://www.baeldung.com/docker-compose) 

* Jeff Nickoloff. **Docker in Action**. Manning, 2016 

*Egon Teiniker, 2025, GPL v3.0*
