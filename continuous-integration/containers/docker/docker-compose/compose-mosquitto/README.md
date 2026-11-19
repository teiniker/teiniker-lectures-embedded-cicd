# Example: Mosquitto MQTT Broker with Docker Compose

In this example we run the same Mosquitto MQTT broker as in the
`docker-container/docker-mosquitto` example. We use the same `Dockerfile`
and `mosquitto.conf`, but replace the `docker build` and `docker run`
commands with a `docker-compose.yml` file.

## Project Structure

```
docker-compose-mosquitto/
├── docker-compose.yml
├── Dockerfile
└── mosquitto.conf
```

The `Dockerfile` and `mosquitto.conf` are copied unchanged from the
`docker-mosquitto` example. Only `docker-compose.yml` is new.


## Mosquitto Configuration

```
listener 1883
allow_anonymous true

log_dest stdout
```

* `listener 1883`: The broker listens on port 1883 on **all** network
    interfaces. Without a `listener`, Mosquitto 2.0 only accepts
    connections from `localhost`, which is the loopback interface
    **inside** the container.

* `allow_anonymous true`: Clients can connect without a username and
    password. This is fine for a local test setup, but not for
    production.

* `log_dest stdout`: Log messages are written to the standard output, so
    we can read them with `docker compose logs`.


## Dockerfile

```dockerfile
FROM eclipse-mosquitto:2.0

COPY mosquitto.conf /mosquitto/config/mosquitto.conf

EXPOSE 1883
```

The `Dockerfile` uses the official Mosquitto image and copies our
configuration file to the location where the image expects it.


## Docker Compose File

```yaml
services:
  mqtt-broker:
    build: .
    image: my-mosquitto
    container_name: mqtt-broker
    ports:
      - "1883:1883"
```

* **Service: `mqtt-broker`**
  - **Build:** `build: .` builds the image from the `Dockerfile` in the
    project directory, like `docker build .`.
  - **Image:** Together with `build`, the `image` entry sets the **name of
    the built image**, like `docker build -t my-mosquitto`. Without it,
    Docker Compose names the image after the project and the service
    (`docker-compose-mosquitto-mqtt-broker`).
  - **Container name:** The container is named `mqtt-broker`, like with
    `docker run --name mqtt-broker`.
  - **Port mapping:** Publishes the MQTT port 1883 on the host, like
    `docker run -p 1883:1883`.


## Run the MQTT Broker

From the project directory, we build the image and start the broker in
the background:

```bash
$ docker compose up -d --build

$ docker compose ps
NAME          IMAGE          COMMAND                  SERVICE       CREATED         STATUS         PORTS
mqtt-broker   my-mosquitto   "/docker-entrypoint.…"   mqtt-broker   5 seconds ago   Up 4 seconds   0.0.0.0:1883->1883/tcp
```

* `-d` (`--detach`): Runs the container in the background.

* `--build`: Builds the image before the container is started. Without
    it, Docker Compose builds the image only if it doesn't exist yet.

The configuration file is part of the image. So when we change
`mosquitto.conf`, we have to rebuild the image with
`docker compose up -d --build`. Docker Compose then replaces the running
container with a new one.

The log output shows that our configuration file was loaded:

```bash
$ docker compose logs mqtt-broker
mqtt-broker  | 1795104264: mosquitto version 2.0.23 starting
mqtt-broker  | 1795104264: Config loaded from /mosquitto/config/mosquitto.conf.
mqtt-broker  | 1795104264: Opening ipv4 listen socket on port 1883.
mqtt-broker  | 1795104264: Opening ipv6 listen socket on port 1883.
mqtt-broker  | 1795104264: mosquitto version 2.0.23 running
```

The numbers at the beginning of each line are Unix timestamps.


## Using the MQTT Broker from the Command Line

### Subscriber

Subscribe to a topic, for example `sensors/distance`:

```bash
$ mosquitto_sub -h 127.0.0.1 -t sensors/distance -d
```

### Publisher

In a second terminal, publish a message:

```bash
$ mosquitto_pub -h 127.0.0.1 -t sensors/distance -m "10cm" -d
```

The subscriber terminal shows the message `10cm`.

**Alternative:** Use the Mosquitto clients which are part of the broker
image. With `docker compose exec`, we use the **service name** instead of
the container name:

```bash
# Subscriber
$ docker compose exec mqtt-broker mosquitto_sub -t sensors/distance -d

# Publisher
$ docker compose exec mqtt-broker mosquitto_pub -t sensors/distance -m "10cm" -d
```

Inside the container, the clients connect to `localhost` by default, so
we don't need the `-h` option.


## Clean Up

Stop and remove the container and the network:

```bash
$ docker compose down
```

This single command replaces `docker container stop` and
`docker container rm`. The images `my-mosquitto` and
`eclipse-mosquitto:2.0` remain on the host and can be removed with:

```bash
$ docker image rm my-mosquitto eclipse-mosquitto:2.0
```


## References

* [Mosquitto Documentation](https://mosquitto.org/documentation/)
* [mosquitto.conf man page](https://mosquitto.org/man/mosquitto-conf-5.html)
* [DockerHub: eclipse-mosquitto](https://hub.docker.com/_/eclipse-mosquitto)
* [Docker Compose: build](https://docs.docker.com/reference/compose-file/build/)

_Egon Teiniker, 2025-2026, GPL v3.0_
