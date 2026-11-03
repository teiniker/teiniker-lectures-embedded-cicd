# MQTT Clients in C++

**Eclipse Paho** also provides an MQTT client library for C++
(`paho.mqtt.cpp`). It is a thin, object-oriented wrapper around the
Paho C library (`paho.mqtt.c`) and offers the same concepts we know
from the Python client: a client object, connect options, callbacks,
publish and subscribe.

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

#include <mqtt/async_client.h>

const std::string SERVER_URI = "tcp://127.0.0.1:1883";
const std::string CLIENT_ID = "cxx-publisher";

// Event handler: the library calls these methods on its own thread.
class PublisherCallback : public mqtt::callback
{
public:
    void connected(const std::string&) override
    {
        std::cout << "Connected to " << SERVER_URI << std::endl;
    }
};

int main()
{
    try
    {
        mqtt::async_client client(SERVER_URI, CLIENT_ID);

        PublisherCallback callback;
        client.set_callback(callback);

        auto options = mqtt::connect_options_builder()
                           .keep_alive_interval(std::chrono::seconds(60))
                           .clean_session()
                           .finalize();

        client.connect(options)->wait();

        // Publish the message
        const std::string TOPIC = "sensors/distance";

// Event handler: the library calls these methods on its own thread.
class SubscriberCallback : public mqtt::callback
{
public:
    explicit SubscriberCallback(mqtt::async_client& client)
        : client_(client)
    {
    }

    void connected(const std::string&) override
    {
        std::cout << "Connected to " << SERVER_URI << std::endl;
        client_.subscribe(TOPIC, 0);
        std::cout << "Subscribed to topic: " << TOPIC << std::endl;
    }

    void message_arrived(mqtt::const_message_ptr msg) override
    {
        std::cout << "Received message on " << msg->get_topic() << ": "
                  << msg->to_string() << std::endl;
    }

private:
    mqtt::async_client& client_;
};
        const std::string MESSAGE = "7cm";
        client.publish(TOPIC, MESSAGE)->wait();
        std::cout << "Published '" << MESSAGE << "' to topic '" << TOPIC
                  << "'" << std::endl;

        // Disconnect cleanly
        client.disconnect()->wait();
    }
    catch (const mqtt::exception& e)
    {
        std::cerr << "MQTT error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
```

* `mqtt::async_client`: Creates the MQTT client object.
    - `SERVER_URI`: protocol, host and port of the broker
        (`tcp://` for plain MQTT, `ssl://` for MQTT over TLS).
    - `CLIENT_ID`: unique name of the client at the broker.
        Two clients with the same ID kick each other off the broker.

* `PublisherCallback`: Derived from `mqtt::callback`, which declares
    one virtual method per event (`connected()`, `connection_lost()`,
    `message_arrived()`, `delivery_complete()`). The base class provides
    empty default implementations, so we only override the events we
    are interested in.
    - `connected()`: Invoked when the connection to the broker is
        established. This corresponds to `on_connect()` in Python.
        The parameter (the cause) is unused, so it has no name.

* `client.set_callback(callback)`: Registers the callback object.
    The client stores only a **reference**, so the object must stay
    alive as long as the client uses it.

* `mqtt::connect_options_builder`: Configures the connection.
    - `keep_alive_interval(60s)`: Pings the broker every 60 seconds
        to maintain the connection.
    - `clean_session()`: The broker does not keep any session state
        (subscriptions, queued messages) for this client.

* `client.connect()`: Opens a TCP connection to the MQTT broker.

* `client.publish()`: Sends the MQTT PUBLISH packet to the broker.

* `client.disconnect()`: Sends an MQTT DISCONNECT packet so the broker
    knows the session ended normally.

The `async_client` works **asynchronously**: every operation returns
immediately with a **token** (`mqtt::token_ptr`). Calling `wait()` on
the token blocks until the operation has completed. If an operation
fails (for example, the broker is not reachable), `wait()` throws an
`mqtt::exception`.

In Python, `connect()` returns a result code which is reported in
`on_connect()`. In C++, errors are reported as exceptions instead.


## Subscriber

```C++
#include <iostream>
#include <string>

#include <mqtt/async_client.h>

const std::string SERVER_URI = "tcp://127.0.0.1:1883";
const std::string CLIENT_ID = "cxx-subscriber";
const std::string TOPIC = "sensors/distance";

// Event handler: the library calls these methods on its own thread.
class SubscriberCallback : public mqtt::callback
{
public:
    explicit SubscriberCallback(mqtt::async_client& client)
        : client_(client)
    {
    }

    void connected(const std::string&) override
    {
        std::cout << "Connected to " << SERVER_URI << std::endl;
        client_.subscribe(TOPIC, 0);
        std::cout << "Subscribed to topic: " << TOPIC << std::endl;
    }

    void message_arrived(mqtt::const_message_ptr msg) override
    {
        std::cout << "Received message on " << msg->get_topic() << ": "
                  << msg->to_string() << std::endl;
    }

private:
    mqtt::async_client& client_;
};

int main()
{
    try
    {
        mqtt::async_client client(SERVER_URI, CLIENT_ID);

        SubscriberCallback callback(client);
        client.set_callback(callback);

        auto options = mqtt::connect_options_builder()
                           .keep_alive_interval(std::chrono::seconds(60))
                           .clean_session()
                           .automatic_reconnect()
                           .finalize();

        client.connect(options)->wait();

        // Callbacks run on a background thread of the library,
        // the main thread just waits until the user presses ENTER.
        std::cout << "Press ENTER to quit..." << std::endl;
        std::cin.get();

        client.disconnect()->wait();
    }
    catch (const mqtt::exception& e)
    {
        std::cerr << "MQTT error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
```

* `SubscriberCallback`: Gets a reference to the client in its
    constructor, because `connected()` has to call `subscribe()`.

* `connected()`: Runs every time the client connects to
    the broker (also after an automatic reconnect).
    - `client_.subscribe(TOPIC, 0)`: Subscribing here ensures that the
        subscription is renewed after every (re)connect.
        The second argument is the QoS level.
    - Inside a callback we must **not** call `wait()` on a token,
        because the callback runs on the library thread that would
        complete the token (deadlock).

* `message_arrived()`: Runs every time a message arrives on any
    subscribed topic. This corresponds to `on_message()` in Python.
    - `msg->get_topic()`: topic name
    - `msg->to_string()`: payload as `std::string`
        (`msg->get_payload()` returns the raw bytes)

* `automatic_reconnect()`: The library reconnects automatically if the
    connection to the broker is lost.

* `std::cin.get()`: In Python, `client.loop_forever()` blocks the main
    thread and processes the network traffic. The Paho C++ library
    already uses its own background thread for networking and
    callbacks, so the main thread only has to stay alive. Here it waits
    until ENTER is pressed and then disconnects cleanly.


## Running the Example

Start the subscriber in one terminal:

```bash
$ ./build/subscriber
Connected to tcp://127.0.0.1:1883
Subscribed to topic: sensors/distance
Press ENTER to quit...
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
