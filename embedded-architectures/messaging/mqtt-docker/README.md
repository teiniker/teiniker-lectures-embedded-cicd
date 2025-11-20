# Docker MQTT Broker  

## Dockerfile

On our Linux VM the **Docker Engine** (dockerd) is not started automatically to conserve 
resources. Enable and start it with systemd:

```bash
$ sudo systemctl start docker
$ sudo systemctl status docker
```

In the given example, we run a Mosquitto MQTT broker in a Docker container.

```dockerfile
FROM eclipse-mosquitto:2.0

COPY mosquitto.conf /mosquitto/config/mosquitto.conf

EXPOSE 1883
```

## Build the Docker Image

Build the MQTT Docker Image as configured in the `Dockerfile`:

```bash
$ docker build -t my-mosquitto .
```

## Create Docker Container

Start mqtt-broker container:

```bash
$ docker run -d --name mqtt-broker -p 1883:1883 my-mosquitto
```

## Using the MQTT Broker from the Command Line

First we have to install mosquitto-clients on our host: 

```bash
$ sudo apt update
$ sudo apt install -y mosquitto-clients
```

This will install both mosquitto_sub (subscriber) and mosquitto_pub (publisher) utilities, allowing you to interact with the MQTT broker from the command line.

### Subscriber 

Subscribe to a topic, f.e `sensors/distance`: 

```bash
$ mosquitto_sub -h 127.0.0.1 -t sensors/distance -d
```

### Publisher 

Publish a message:

```bash
$ mosquitto_pub -h 127.0.0.1 -t sensors/distance -m "10cm" -d
```


**Alternativ**: Use the Mosquitto Client in a Docker Container:

```bash
# Subscriber
docker exec -it mqtt-broker mosquitto_sub -t sensors/distance -d

# Publisher
docker exec -it mqtt-broker mosquitto_pub -t sensors/distance -m "10cm" -d
``` 

## Clean Up 

Stop and remove the container:

```bash
# Stop and remove the container
$ docker container stop mqtt-broker
$ docker container rm mqtt-broker
```

## References

* [Mosquitto Documentation](https://mosquitto.org/documentation/)
* [DockerHub: eclipse-mosquitto](https://hub.docker.com/_/eclipse-mosquitto)

_Nicoletta Kähling, Egon Teiniker, 2025, GPL v3.0_






