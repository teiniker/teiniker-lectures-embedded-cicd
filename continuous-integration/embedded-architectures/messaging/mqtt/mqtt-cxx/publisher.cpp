#include <iostream>
#include <string>

#include <mqtt/async_client.h>

// mosquitto_pub -h 127.0.0.1 -t sensors/distance -m "10cm"
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
