# Using Mosquitto on the Command-Line

The Mosquitto project also provides a **C library** for implementing MQTT 
clients, and the very popular **mosquitto_pub** and **mosquitto_sub** 
**command line MQTT clients**.


## Subscribing to Topics

We will use the **mosquitto_sub** command-line utility included in Mosquitto 
to generate a simple MQTT client that subscribes to a topic and prints all the 
messages it receives.

```bash
$ mosquitto_sub  -t sensors/distance -d

Client (null) sending CONNECT
Client (null) received CONNACK (0)
Client (null) sending SUBSCRIBE (Mid: 1, Topic: sensors/distance, QoS: 0, Options: 0x00)
Client (null) received SUBACK
Subscribed (mid: 1): 0
```

Parameters:
* `-t`: Topic the client subscribes to
* `-d`: Debugging mode 
* `-V` mqttv311: Version number 
* `-h localhost` 

The Terminal window will display messages published to the topic to which we subscribed 
as they arrive from the MQTT server to the MQTT client.

The client sends PINGREQ packets to the MQTT server and receives PINQRESP packets 
from the MQTT server.

```bash
Client (null) sending PINGREQ
Client (null) received PINGRESP
```

## Publishing Messages

We will use the **mosquitto_pub** command-line utility included in Mosquitto to 
generate a simple MQTT client that publishes a message to a topic.

```bash
$ mosquitto_pub -t sensors/distance -m  "10cm" -d
Client (null) sending CONNECT
Client (null) received CONNACK (0)
Client (null) sending PUBLISH (d0, q0, r0, m1, 'sensors/distance', ... (4 bytes))
Client (null) sending DISCONNECT
```

Parameters:
* `-t`: Topic the client subscribes to
* `-m`: Message
* `-d`: Debugging mode 
* `-V mqttv311`: Version number 
* `-h localhost` 

The previous command will create an MQTT client that will establish a connection with 
the local MQTT server and then will make the client publish a message to the topic 
specified after the `-t` option.

After publishing the message, the client disconnects.


## References

* [Eclipse Mosquitto](https://mosquitto.org/)

_Egon Teiniker, 2025-2026, GPL v3.0_