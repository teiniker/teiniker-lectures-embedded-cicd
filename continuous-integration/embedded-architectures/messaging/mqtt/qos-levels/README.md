# MQTT QoS Levels

**Quality of Service (QoS)** defines how reliably a message is delivered between 
clients and the broker. There are three levels: QoS 0, QoS 1, and QoS 2.

* **QoS 0 - “At most once”**: Fire-and-forget.
    - The message is delivered at most once, with no acknowledgment.
    - No retries.
    - Fastest and lightest, but least reliable.
    - Use cases: sensor data sent frequently, where occasional loss is OK.

* **QoS 1 — “At least once”**: Guaranteed delivery, but possible duplicates.
    - Broker must acknowledge receipt (PUBACK).
    - If the sender doesn't get PUBACK, it retries.
    - Ensures the message arrives, but may arrive more than once.
    - Use cases: Most IoT events where you want reliability but duplication is acceptable.

* **QoS 2 — “Exactly once”**: Most reliable, ensures no duplicates.
    - Uses a four-step handshake (PUBLISH > PUBREC > PUBREL > PUBCOMP).
    - Heaviest in terms of bandwidth and latency.
    - Use cases: Billing systems, financial transactions, mission-critical data.


**Mosquitto’s command-line clients** (`mosquitto_pub` and `mosquitto_sub`) use: 
    **QoS 0 (at most once)**

We can use the **-q option** to specify the QoS level:

```bash 
# Subscribe with QoS 1
$ mosquitto_sub -t "home/temperature" -q 1

# Publish with QoS 1
$ mosquitto_pub -t "home/temperature" -m "23" -q 1

# Subscribe with QoS 2
$ mosquitto_sub -t "home/temperature" -q 2

# Publish with QoS 2
$ mosquitto_pub -t "home/temperature" -m "23" -q 2
```

## References
* Gaston C. Hillar.
	**Hands-On MQTT Programming with Python: Work with the lightweight IoT protocol in Python**.
	Packt Publishing, Kindle-Version, 2018

* [What is MQTT and How It Works](https://randomnerdtutorials.com/what-is-mqtt-and-how-it-works/)

_Egon Teiniker, 2025-2026, GPL v3.0_
