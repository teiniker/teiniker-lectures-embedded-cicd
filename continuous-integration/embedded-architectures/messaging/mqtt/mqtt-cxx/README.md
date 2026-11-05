# MQTT Clients in C++

**Eclipse Paho** also provides an MQTT client library for C++
(`paho.mqtt.cpp`). It is a thin, object-oriented wrapper around the
Paho C library (`paho.mqtt.c`) and offers the same concepts we know
from the Python client: a client object, connect options, publish
and subscribe.

The library contains two clients:

* `mqtt::client` (header `<mqtt/client.h>`): A **synchronous** client.
    Every method blocks until the operation has completed.

* `mqtt::async_client` (header `<mqtt/async_client.h>`): An
    **asynchronous** client. Every method returns immediately with a
    token, which can be used to wait for the result later.

The synchronous client is a thin wrapper around an `async_client`:
every method starts the asynchronous operation and waits for its
result. We use the synchronous client, because the program reads
like the Python version.

## Setup

On Debian/Ubuntu, the C++ library and the C library it depends on
can be installed as packages:

```bash
$ sudo apt install libpaho-mqttpp-dev libpaho-mqtt-dev
```

Note that `libpaho-mqttpp-dev` does not pull in `libpaho-mqtt-dev`
automatically. Without it, `find_package(PahoMqttCpp)` fails because
the CMake config file `eclipse-paho-mqtt-cConfig.cmake` is missing.

The package provides a CMake config file, so the library can be found
with `find_package()`:

```CMake
find_package(PahoMqttCpp REQUIRED)

add_executable(publisher publisher.cpp)
target_link_libraries(publisher PRIVATE PahoMqttCpp::paho-mqttpp3)

add_executable(subscriber subscriber.cpp)
target_link_libraries(subscriber PRIVATE PahoMqttCpp::paho-mqttpp3)
```

Build both clients:

```bash
$ cmake -S . -B build
$ cmake --build build
```




## Publisher

```C++
#include <iostream>
#include <string>

#include <mqtt/client.h>

using namespace std;

// mosquitto_pub -h 127.0.0.1 -t sensors/distance -m "10cm"
const string SERVER_URI = "tcp://127.0.0.1:1883";
const string CLIENT_ID = "cxx-publisher";

int main()
{
    try
    {
        mqtt::client client(SERVER_URI, CLIENT_ID);

        auto options = mqtt::connect_options_builder()
                           .keep_alive_interval(chrono::seconds(60))
                           .clean_session()
                           .finalize();

        // Every call blocks until the operation has completed.
        client.connect(options);
        cout << "Connected to " << SERVER_URI << endl;

        // Publish the message
        const string TOPIC = "sensors/distance";
        const string MESSAGE = "7cm";
        client.publish(mqtt::make_message(TOPIC, MESSAGE));
        cout << "Published '" << MESSAGE << "' to topic '" << TOPIC
             << "'" << endl;

        // Disconnect cleanly
        client.disconnect();
    }
    catch (const mqtt::exception& e)
    {
        cerr << "MQTT error: " << e.what() << endl;
        return 1;
    }
    return 0;
}
```

* `mqtt::client`: Creates the MQTT client object.
    - `SERVER_URI`: protocol, host and port of the broker
        (`tcp://` for plain MQTT, `ssl://` for MQTT over TLS).
    - `CLIENT_ID`: unique name of the client at the broker.
        Two clients with the same ID kick each other off the broker.

* `mqtt::connect_options_builder`: Configures the connection.
    - `keep_alive_interval(60s)`: Pings the broker every 60 seconds
        to maintain the connection.
    - `clean_session()`: The broker does not keep any session state
        (subscriptions, queued messages) for this client.

* `client.connect()`: Opens a TCP connection to the MQTT broker.

* `client.publish()`: Sends the MQTT PUBLISH packet to the broker.
    - `mqtt::make_message(TOPIC, MESSAGE)`: Creates the message object.
        Optional arguments set the QoS level and the retained flag:
        `mqtt::make_message(TOPIC, MESSAGE, 1, false)`.

* `client.disconnect()`: Sends an MQTT DISCONNECT packet so the broker
    knows the session ended normally.

Each of these calls **blocks** until the operation has completed.
If an operation fails (for example, the broker is not reachable),
an `mqtt::exception` is thrown. Internally, the client waits with a
timeout which can be changed with `client.set_timeout()`. If an
operation takes longer, an `mqtt::timeout_error` is thrown.

In Python, `connect()` returns a result code which is reported in
`on_connect()`. In C++, errors are reported as exceptions instead.


## Subscriber

```C++
#include <iostream>
#include <string>

#include <mqtt/client.h>

using namespace std;

const string SERVER_URI = "tcp://127.0.0.1:1883";
const string CLIENT_ID = "cxx-subscriber";
const string TOPIC = "sensors/distance";

int main()
{
    try
    {
        mqtt::client client(SERVER_URI, CLIENT_ID);

        auto options = mqtt::connect_options_builder()
                           .keep_alive_interval(chrono::seconds(60))
                           .clean_session()
                           .finalize();

        // Incoming messages are stored in a queue instead of
        // being passed to a callback.
        client.start_consuming();

        client.connect(options);
        cout << "Connected to " << SERVER_URI << endl;

        client.subscribe(TOPIC, 0);
        cout << "Subscribed to topic: " << TOPIC << endl;

        // Blocking loop: wait for the next message in the queue.
        while (true)
        {
            auto msg = client.consume_message();
            if (!msg)   // nullptr: connection to the broker was lost
            {
                cerr << "Connection lost" << endl;
                break;
            }
            cout << "Received message on " << msg->get_topic() << ": "
                 << msg->to_string() << endl;
        }

        client.stop_consuming();
    }
    catch (const mqtt::exception& e)
    {
        cerr << "MQTT error: " << e.what() << endl;
        return 1;
    }
    return 0;
}
```

* `client.start_consuming()`: The library puts every incoming message
    into an internal queue. This must be called **before** `connect()`,
    so that no message is lost.

* `client.subscribe(TOPIC, 0)`: Blocks until the broker has confirmed
    the subscription. The second argument is the QoS level.

* `client.consume_message()`: Takes the next message from the queue.
    If the queue is empty, the call blocks until a message arrives.
    This loop corresponds to `client.loop_forever()` in Python.
    - `msg->get_topic()`: topic name
    - `msg->to_string()`: payload as `std::string`
        (`msg->get_payload()` returns the raw bytes)
    - It returns `nullptr` if the connection to the broker is lost.
    - `try_consume_message_for(&msg, timeout)` waits at most `timeout`
        and returns `false` if no message arrived. This is useful in
        a main loop which also has other work to do.

* `client.stop_consuming()`: Stops putting messages into the queue.

The subscriber runs until the connection is lost or it is stopped
with `Ctrl+C`. All our code runs on the main thread, there are no
callbacks which run on a library thread.


## Running the Example

Start the subscriber in one terminal:

```bash
$ ./build/subscriber
Connected to tcp://127.0.0.1:1883
Subscribed to topic: sensors/distance
Received message on sensors/distance: 7cm
Received message on sensors/distance: 10cm
```

Publish messages from a second terminal, either with the C++ publisher
or with the Mosquitto command line client:

```bash
$ ./build/publisher
Connected to tcp://127.0.0.1:1883
Published '7cm' to topic 'sensors/distance'

$ mosquitto_pub -h 127.0.0.1 -t sensors/distance -m "10cm"
```

Since both sides only speak MQTT, the C++ clients can be freely
combined with the Python clients from `mqtt-python`.




## References

* [paho](https://eclipse.dev/paho/)
* [GitHub: Eclipse Paho MQTT C++ Client Library](https://github.com/eclipse-paho/paho.mqtt.cpp)

_Egon Teiniker, 2025-2026, GPL v3.0_
