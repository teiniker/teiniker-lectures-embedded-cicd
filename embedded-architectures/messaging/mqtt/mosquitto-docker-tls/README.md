# Mosquitto MQTT Using TLS in a Docker Container 

To enable secure communication between the MQTT broker and its publishers  
and subscribers, we use a Transport Layer Security (TLS) connection.

To establish a TLS connection, we need a digital certificate.  
For testing purposes, we use a self-signed certificate.


## Generate TLS Certificates

We use **OpenSSL** to generate **self-signed certificates** for this example.

Nevertheless, we keep a separation between the **Certificate Authority (CA)**  
and the generated certificate. In this case, clients can use the CA  
certificate to establish the connection to the MQTT broker and perform  
the TLS handshake.

```bash
$ mkdir certs
$ cd certs

# Create a CA - Certificate Authority
$ openssl genrsa -out ca.key 2048
$ openssl req -x509 -new -nodes -key ca.key -sha256 -days 1024 -out ca.crt -subj "/CN=TestCA"

# Create Server Certificate
$ openssl genrsa -out server.key 2048
$ openssl req -new -key server.key -out server.csr -subj "/CN=localhost"
$ openssl x509 -req -in server.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out server.crt -days 500 -sha256

$ tree
.
├── ca.crt      # Public certificate of our Certificate Authority (CA)  
├── ca.key      # Private key of your CA
├── ca.srl      # Certificate Serial Number file
├── server.crt  # Signed certificate for your server
├── server.csr  # Certificate Signing Request (CSR)
└── server.key  # Private key of your server
```

## Start the Mosquitto Broker with Docker Compose

We use Docker Compose here because it simplifies the configuration  
of the Docker container.

The following `docker-compose.yml` file shows all required settings:

```yaml
services:
  mosquitto:
    image: eclipse-mosquitto:2.0
    container_name: mqtt-broker
    ports:
      - "1883:1883"   # insecure MQTT port
      - "8883:8883"   # TLS port
    volumes:
      - ./config/mosquitto.conf:/mosquitto/config/mosquitto.conf:ro
      - ./data:/mosquitto/data
      - ./certs:/mosquitto/certs
    restart: unless-stopped
```

* **Volumes (bind mounts)**: Mount files on our host filesystem accessible 
    inside the container.
    - **Mosquitto configuration file**: `./config/mosquitto.conf:/mosquitto/config/mosquitto.conf:ro`
        - Mounted read-only (ro)
    - **Persistent Mosquitto data**: `./data:/mosquitto/data` 
        - Stores broker data such as: persistence database (retained messages), 
            session data, offline message queue 
    - **Certificates directory**: `./certs:/mosquitto/certs`
        - Makes our TLS certificates (`ca.crt`, `server.crt`, `server.key`) available 
        in the container.

* **Restart policy**: `restart: unless-stopped` 
    - Automatically restart the container if it crashes
    - Restart on machine boot
    - Do not restart only if the user manually stops it (`docker stop`)


```bash 
# Create and run mqtt-broker as Docker container
$ docker compose up -d

# follow the live output (logs) of the mqtt-broker Docker container
$ docker logs -f mqtt-broker
```

## TLS Publisher and Subscriber 

With the CA certificate, publishers and subscribers can connect  
to the server. The public key of the CA certificate is used  
to verify the server's certificate.

```bash
$ mosquitto_sub -h localhost -p 8883 --cafile certs/ca.crt -t sensors/distance -d

$ mosquitto_pub -h localhost -p 8883 --cafile certs/ca.crt -t sensors/distance -m "10cm" -d
```

* `-p 8883`: Sets the TCP port to 8883 which is the standard port for secure MQTT over TLS
    (default insecure MQTT uses port 1883)

* `--cafile certs/ca.crt`: Trust this CA certificate when verifying the broker’s TLS certificate.
    - ca.crt is your Certificate Authority’s public certificate
    - It is used to validate the broker’s certificate (server.crt)

    Without this file, TLS connection will fail because the client cannot trust the server.


In the case that we only use a **simple self‑signed certificate**,  
we must use the `--insecure` flag on the client side.

```bash
$ mosquitto_sub -h localhost -p 8883 --cafile certs/ca.crt --insecure -t sensors/distance -d
```


## References

* [Mosquitto: TLS](https://mosquitto.org/man/mosquitto-tls-7.html)

* [Mosquitto Documentation](https://mosquitto.org/documentation/)
* [DockerHub: eclipse-mosquitto](https://hub.docker.com/_/eclipse-mosquitto)

_Nicoletta Kähling, Egon Teiniker, 2025, GPL v3.0_
