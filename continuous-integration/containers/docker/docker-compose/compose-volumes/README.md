# Configuring Volumes

There are three types of volumes:

* Docker manages both **anonymous and named volumes**, automatically mounting 
  them in self-generated directories in the host. While anonymous volumes were 
  useful with older versions of Docker (pre 1.9), named ones are the 
  suggested way to go nowadays. 

* **Host volumes** allow us to specify an existing folder in the host.

We can configure **host volumes at the service level** and 
**named volumes in the outer level** of the configuration, 
in order to make the latter visible to other containers and not only 
to the one they belong:

```
services:
  volumes-example-service:
    image: alpine:latest
    volumes: 
      - my-named-global-volume:/my-volumes/named-global-volume
      - /tmp:/my-volumes/host-volume
      - /home:/my-volumes/readonly-host-volume:ro
    ...
  another-volumes-example-service:
    image: alpine:latest
    volumes:
      - my-named-global-volume:/another-path/the-same-named-global-volume
    ...
volumes:
  my-named-global-volume: 
```

Both containers will have read/write access to the `my-named-global-volume` 
shared folder, no matter the different paths they've mapped it to. 

The two host volumes, instead, will be available only to `volumes-example-service`.

The `/tmp` folder of the host's file system is mapped to the 
`/my-volumes/host-volume` folder of the container.
This portion of the file system is writeable, which means that the container 
can not only read but also write (and delete) files in the host machine.

We can mount a volume in read-only mode by appending `:ro` to the rule, like 
for the `/home` folder.


## References
* [Introduction to Docker Compose](https://www.baeldung.com/docker-compose) 
* [Overview of Docker Compose](https://docs.docker.com/compose/)

* Jeff Nickoloff. **Docker in Action**. Manning, 2016 

*Egon Teiniker, 2025, GPL v3.0*