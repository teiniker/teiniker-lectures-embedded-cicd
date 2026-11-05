# MQTT Chat in Python

A simple chat built on the publish/subscribe pattern: every participant
publishes text messages to the topic `mqtt/chat`, and every subscriber of
this topic receives and prints them.

The broker decouples the participants: publishers don't know who is
listening, and subscribers don't know who is sending.

## Setup

We use a virtual environment to install the required libraries:

```bash
$ pip install paho-mqtt
```

A running MQTT broker (for example Mosquitto) is required:

```bash
$ mosquitto -p 1883
```


## Subscriber

The subscriber connects to the broker, subscribes to the topic `mqtt/chat`
and prints every received message with a timestamp to the console.

```Python
import datetime
import paho.mqtt.client as mqtt

BROKER = "127.0.0.1"
PORT = 1883
TOPIC = "mqtt/chat"

def on_connect(client, userdata, flags, reason_code, properties):
    if reason_code.is_failure:
        print(f"Connection failed: {reason_code}")
        return
    print(f"Connected to {BROKER}:{PORT}")
    # Subscribe here, so the subscription is renewed after a reconnect
    client.subscribe(TOPIC)
    print(f"Listening on topic '{TOPIC}' (press Ctrl+C to quit)")

def on_message(client, userdata, msg):
    timestamp = datetime.datetime.now().strftime("%H:%M:%S")
    text = msg.payload.decode("utf-8", errors="replace")
    print(f"[{timestamp}] {text}")

client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
client.on_connect = on_connect
client.on_message = on_message

client.connect(BROKER, PORT, keepalive=60)

try:
    # Blocking loop to process network traffic and dispatch callbacks
    client.loop_forever()
except KeyboardInterrupt:
    print("\nChat closed.")
    client.disconnect()
```

* `mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)`: Creates the MQTT client
    using version 2 of the callback API (Paho 2.x). Without this argument,
    Paho falls back to the deprecated version 1 and prints a warning.

* `on_connect()`: Runs when the broker answers the connection request.
    - `reason_code`: result of the connection attempt;
        `reason_code.is_failure` is `True` if the broker rejected it.
    - `properties`: MQTT v5 properties (not used here).
    - `client.subscribe(TOPIC)`: Subscribing in this callback ensures that
        the subscription is restored after an automatic reconnect.

* `on_message()`: Runs for every message published to `mqtt/chat`.
    - `msg.payload` contains bytes, so we decode it as UTF-8.
        `errors="replace"` prevents a crash on invalid byte sequences.

* `client.loop_forever()`: Blocking loop which processes network traffic
    and dispatches the callbacks.

* `KeyboardInterrupt`: Pressing Ctrl+C ends the loop, and
    `client.disconnect()` closes the session cleanly.

Start the subscriber:

```bash
$ python chat_subscriber.py
Connected to 127.0.0.1:1883
Listening on topic 'mqtt/chat' (press Ctrl+C to quit)
```

You can start several subscribers in different terminals: each of them
receives a copy of every message.


## Publisher (Command Line)

Instead of writing a publisher in Python, we use the command line tool
`mosquitto_pub`, which is part of the Mosquitto client package
(`mosquitto-clients` on Debian/Ubuntu).

Send a single message:

```bash
$ mosquitto_pub -h 127.0.0.1 -p 1883 -t mqtt/chat -m "Hello everybody!"
```

* `-h`: host name or IP address of the broker
* `-p`: port of the broker (default: 1883)
* `-t`: topic to publish to
* `-m`: message payload

The subscriber prints:

```
[18:26:17] Hello everybody!
```

To send several messages in a row, `mosquitto_pub` can read from standard
input. With `-l` every line typed into the terminal is published as a
separate message (end with Ctrl+D):

```bash
$ mosquitto_pub -h 127.0.0.1 -t mqtt/chat -l
Hi, is anybody there?
Grüß Gott!
```

Since the message has no sender information, a simple convention is to put
the name at the beginning of the text:

```bash
$ mosquitto_pub -h 127.0.0.1 -t mqtt/chat -m "Alice: Hello Bob!"
```

For comparison, `mosquitto_sub` is the command line counterpart of our
Python subscriber:

```bash
$ mosquitto_sub -h 127.0.0.1 -t mqtt/chat
```


## References

* [paho](https://eclipse.dev/paho/)
* [mosquitto_pub](https://mosquitto.org/man/mosquitto_pub-1.html)
* [mosquitto_sub](https://mosquitto.org/man/mosquitto_sub-1.html)

_Egon Teiniker, 2025-2026, GPL v3.0_
