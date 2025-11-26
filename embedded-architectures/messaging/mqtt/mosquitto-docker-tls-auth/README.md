# Mosquitto MQTT Using TLS and Authentication in a Docker Container 

Set up a secure Mosquitto MQTT broker with Docker, using TLS for encryption and authentication with passwords.

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
$ openssl genrsa -out ca.key 4096
$ openssl req -x509 -new -nodes -key ca.key -sha256 -days 1024 -out ca.crt -subj "/CN=TestCA"

# Create Server Certificate
$ openssl genrsa -out server.key 4096
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

## Create Password File

We need a native Mosquitto installation on our host to use the `mosquitto_passwd` tool:

```bash
$ sudo apt install mosquitto
```

Create the directory for the password file:

```bash
$ mkdir auth
```

Create users and set their passwords:

```bash
$ mosquitto_passwd -c auth/passwords homer
$ mosquitto_passwd auth/passwords marge
```

Set the file permissions so Mosquitto can read it:

```bash
$ chmod 644 auth/passwords
```

```bash
$ cat ./auth/passwords
homer:$7$101$pPmJ4Nn/EtNUWR5M$KftBLTEynldi63eZJ3NGvkaXtvWrdRCrYT4uEqKdqOsM61duU2Mbhpd5xNYV56P41VsKIPRJfKtFsPpFl3Nq/w==
marge:$7$101$mlJfzwH/D5/SPxFB$QOrQgrAREXtxCxFsv3ExIegkvVHuuUG2Z52IZwPaZj3YioDG6MXLuDb3mzRh8XydvCXVUr/6ElXvuxcbwFYnjQ==
```

Mosquitto uses **scrypt** to store passwords.
A strong, modern, memory-hard algorithm designed to resist GPU and ASIC cracking.

The prefix `$7$` is the identifier for scrypt in the "modular crypt format" 
(MCF) used by several systems (notably libscrypt, OpenBSD, various PAM modules, 
and some Linux distributions):

_Example:_ `$7$101$pPmJ4Nn/EtNUWR5M$KftBLTEynldi63eZJ3NGvkaXtvWrdRCrYT4uEqKdqOsM61duU2Mbhpd5xNYV56P41VsKIPRJfKtFsPpFl3Nq/w==`
* `$7$`: scrypt identifier
* `101`: scrypt cost parameters (encoded)
* `pPmJ4Nn/EtNUWR5M`: Base64-encoded salt
* `KftBLT...Nq/w==`: Base64-encoded scrypt hash



## Start the Mosquitto Broker with Docker Compose

The following `docker-compose.yml` file shows all required settings:

```yaml
services:
  mosquitto:
    image: eclipse-mosquitto:2.0
    container_name: mqtt-broker
    ports:
      - "8883:8883"
    volumes:
      - ./config/mosquitto.conf:/mosquitto/config/mosquitto.conf
      - ./data:/mosquitto/data
      - ./certs:/mosquitto/certs
      - ./auth:/mosquitto/auth
    restart: unless-stopped
```

```bash
# Create and run mqtt-broker as Docker container
$ docker compose up -d

# follow the live output (logs) of the mqtt-broker Docker container
$ docker logs -f mqtt-broker
```


## TLS Subscriber with Authentication 

```bash
$ mosquitto_sub -h localhost -p 8883 --cafile certs/ca.crt -u homer -P homer -t sensors/distance -d
```

* `-p 8883`: Sets the TCP port to 8883 which is the standard port for secure MQTT over TLS
    (default insecure MQTT uses port 1883)

* `--cafile certs/ca.crt`: Trust this CA certificate when verifying the broker’s TLS certificate.
    - ca.crt is your Certificate Authority’s public certificate
    - It is used to validate the broker’s certificate (server.crt)

    Without this file, TLS connection will fail because the client cannot trust the server.

* * `-p 8883`: Sets the TCP port to 8883 which is the standard port for secure MQTT over TLS
    (default insecure MQTT uses port 1883)

* `--cafile certs/ca.crt`: Trust this CA certificate when verifying the broker’s TLS certificate.
    - ca.crt is your Certificate Authority’s public certificate
    - It is used to validate the broker’s certificate (server.crt)

    Without this file, TLS connection will fail because the client cannot trust the server.

* `-u homer`: Sets the username for MQTT authentication.

* `-P homer`: Sets the password for the user. The broker will verify it against its password file.


### TLS Publisher with Authentication

```bash
$ mosquitto_pub -h localhost -p 8883 --cafile certs/ca.crt -u marge -P marge  -t sensors/distance -m "42cm" -d
```

* `-p 8883`: Sets the TCP port to 8883 which is the standard port for secure MQTT over TLS
    (default insecure MQTT uses port 1883)

* `--cafile certs/ca.crt`: Trust this CA certificate when verifying the broker’s TLS certificate.
    - ca.crt is your Certificate Authority’s public certificate
    - It is used to validate the broker’s certificate (server.crt)

    Without this file, TLS connection will fail because the client cannot trust the server.

* `-p 8883`: Sets the TCP port to 8883 which is the standard port for secure MQTT over TLS
    (default insecure MQTT uses port 1883)

* `--cafile certs/ca.crt`: Trust this CA certificate when verifying the broker’s TLS certificate.
    - ca.crt is your Certificate Authority’s public certificate
    - It is used to validate the broker’s certificate (server.crt)

    Without this file, TLS connection will fail because the client cannot trust the server.

* `-u marge`: Sets the username for MQTT authentication.

* `-P marge`: Sets the password for the user. The broker will verify it against its password file.


In the case that we only use a **simple self‑signed certificate**,  
we must use the `--insecure` flag on the client side.

## References 

* [Mosquitto: Authentication Methods](https://mosquitto.org/documentation/authentication-methods/)
* [Mosquitto: mosquitto_passwd man page](https://mosquitto.org/man/mosquitto_passwd-1.html)

_Nicoletta Kähling, Egon Teiniker, 2025, GPL v3.0_
