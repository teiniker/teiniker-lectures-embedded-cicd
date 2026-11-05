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
