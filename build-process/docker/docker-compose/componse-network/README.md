# Configuring the Network
Docker containers communicate between themselves in networks created, implicitly 
or through configuration, by Docker Compose. 
A service can communicate with another service on the same network by simply referencing 
it by **container name and port**, provided that we have made the port accessible through 
the `expose` keyword:

```
services:
  network-example-service:
    image: karthequian/helloworld:latest
    expose:
      - "80"
```

To **reach a container from the host**, the ports must be exposed declaratively through 
the `ports` keyword, which also allows us to choose the **port differently in the host**. 
This powerful mechanism allows us to run different containers exposing the same ports 
without collisions.

```
services:
  my-custom-app:
    image: myapp:latest
    ports:
      - "8080:3000"
    ...
```

Finally, we can define **additional virtual networks** to segregate our containers:
```
services:
  network-example-service:
    image: karthequian/helloworld:latest
    networks: 
      - my-shared-network
    ...
  another-service-in-the-same-network:
    image: alpine:latest
    networks: 
      - my-shared-network
    ...
  another-service-in-its-own-network:
    image: alpine:latest
    networks: 
      - my-private-network
    ...
networks:
  my-shared-network: {}
  my-private-network: {}
```
We can see that a`nother-service-in-the-same-network` will be able to ping 
and to reach port 80 of `network-example-service`, while 
`another-service-in-its-own-network` won't.


## References
* [Introduction to Docker Compose](https://www.baeldung.com/docker-compose) 
* [Overview of Docker Compose](https://docs.docker.com/compose/)

* Jeff Nickoloff. **Docker in Action**. Manning, 2016 

*Egon Teiniker, 2025, GPL v3.0*