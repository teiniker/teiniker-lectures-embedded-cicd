# MQTT Clients in Python

**Eclipse Paho** is an open-source project hosted by the Eclipse Foundation 
that provides client libraries for the MQTT (and in some parts for MQTT-SN) 
publish/subscribe messaging protocol.

## Setup

Wir verwenden eine Virtual Environment um die notwendigen Libraries zu installieren:
```bash
# Activate environment
$ python -m venv .venv
$ source .venv/bin/activate

# Install libraries
$ pip install -r requirements.txt

$ pip list
Package   Version
--------- -------
paho-mqtt 2.1.0
pip       25.1.1

# Deactivate environment
$ deactivate
```


## Publisher 
With the Paho library, you can quickly implement an MQTT publisher that  
connects to a broker and publishes messages (for example, strings) to a 
specific topic.

```Python
import paho.mqtt.client as mqtt

BROKER = "127.0.0.1"
PORT = 1883

def on_connect(client, userdata, flags, rc):
    print("Connected with result code:", rc)

client = mqtt.Client()
client.on_connect = on_connect
client.connect(BROKER, PORT, keepalive=60)

# Publish the message
TOPIC = "sensors/distance"
MESSAGE = "7cm"
client.publish(TOPIC, MESSAGE)
print(f"Published '{MESSAGE}' to topic '{TOPIC}'")

# Disconnect cleanly
client.disconnect()
```

* `on_connect()`: This is a callback function invoked automatically 
    when the client connects to the broker.
    - client: the MQTT client instance
    - userdata: custom data you can attach (usually not needed)
    - flags: response flags from the broker
    - rc: connection result code (0 means success, non-zero means 
        some problem (auth error, broker unavailable, etc.)

* `mqtt.Client()`: Creates the MQTT client object

* `client.on_connect`: Registers the on_connect callback 
    so it runs when connected.

* `client.connect()`: Opens a TCP connection to the MQTT broker.
    - keepalive=60: Pings broker every 60 seconds to maintain connection.

* `client.publish()`: Sends the MQTT packet to the broker.

* `client.disconnect()`: Sends an MQTT DISCONNECT packet so the broker 
    knows the session ended normally.



## Subscriber

Using the Paho library, you can also easily implement a subscriber  
that reads data from a specific topic.


```Python
import paho.mqtt.client as mqtt

BROKER = "127.0.0.1"
PORT = 1883
TOPIC = "sensors/distance"

def on_connect(client, userdata, flags, rc):
    print("Connected with result code:", rc)
    client.subscribe(TOPIC)
    print(f"Subscribed to topic: {TOPIC}")

def on_message(client, userdata, msg):
    print(f"Received message on {msg.topic}: {msg.payload.decode()}")

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.connect(BROKER, PORT, keepalive=60)

# Blocking loop to process network traffic and dispatch callbacks
client.loop_forever()
```

* `on_connect()`: A callback function which runs automatically when the 
    client successfully connects to the MQTT broker.
    - `client.subscribe(TOPIC)`: Subscribing here ensures the subscription 
        happens exactly when the broker accepts the connection.
        This is the recommended practice.

* `on_message()`: This callback runs every time a message arrives on any 
    subscribed topic.
    - msg.topic: topic name (string)
    - msg.payload: received data (bytes), so we decode() to string

* `mqtt.Client()`: Creates an MQTT client object and registers two callback 
    functions:
    - client.on_connect
    - client.on_message

* `client.connect()`: Opens a network connection to the MQTT broker.
    - keepalive=60 means the client will send a ping every 60 seconds 
        to keep the connection open.

* `client.loop_forever()`: This starts a blocking loop that:
    - reads incoming network packets
    - processes MQTT protocol messages
    - triggers callbacks (on_message, on_connect, etc.)
    - reconnects automatically if needed

    Without this loop, the script would not receive any messages.
    This line essentially keeps the subscriber running and listening 
    indefinitely.


## References

* [paho](https://eclipse.dev/paho/)

_Egon Teiniker, 2025, GPL v3.0_