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
