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
