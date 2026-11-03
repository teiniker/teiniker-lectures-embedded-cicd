# Setup Mosquitto  

```bash
$ sudo apt update
$ sudo apt install mosquitto mosquitto-clients
```

```bash
$ sudo systemctl status mosquitto
mosquitto.service - Mosquitto MQTT Broker
     Loaded: loaded (/lib/systemd/system/mosquitto.service; enabled; preset: enabled)
     Active: active (running) since Wed 2024-01-03 17:33:14 CET; 18s ago
```

We don't want to start Mosquitto MQTT Message Broker automatically when the Linux boots.

```bash
$ sudo systemctl disable mosquitto
// $ sudo systemctl enable mosquitto     // default setting 

$ sudo systemctl start mosquitto
$ sudo systemctl stop mosquitto
```


## Accessing the Linux VM 

In order to access the Linux VM from the local network, the network mode 
of the VM must be set to **Bridged Networking** (the default setting is NAT).

You can find out the IP address of the Linux VM with the following command:

```bash
$ hostname -I
192.168.0.38 
```

For example, you can access the Linux VM from the Windows host:

```bash
> ping 192.168.0.38

Ping wird ausgeführt für 192.168.0.38 mit 32 Bytes Daten:
Antwort von 192.168.0.38: Bytes=32 Zeit<1ms TTL=64
Antwort von 192.168.0.38: Bytes=32 Zeit<1ms TTL=64
Antwort von 192.168.0.38: Bytes=32 Zeit<1ms TTL=64
Antwort von 192.168.0.38: Bytes=32 Zeit<1ms TTL=64

Ping-Statistik für 192.168.0.38:
    Pakete: Gesendet = 4, Empfangen = 4, Verloren = 0
    (0% Verlust),
Ca. Zeitangaben in Millisek.:
    Minimum = 0ms, Maximum = 0ms, Mittelwert = 0ms
```

An SSH connection can also be established:

```bash
> ssh student@192.168.0.38
student@192.168.0.38's password:
student@debian:~$
```

## Manage the Linux Firewall with Uncomplicated Firewall (UFW)

UFW is a user-friently, secure, and flexible firewall for Debian Linux.

First, update your APT repos and install any pending updates:

```bash
$ sudo apt update
$ sudo apt upgrade
```

Next, we install UFW using the `apt` command:

```bash
$ sudo apt install ufw

$ sudo ufw version
[sudo] password for student: 
ufw 0.36.2
Copyright 2008-2023 Canonical Ltd.
```

Now, we can configure UFW to allow SSH, MQTT (HTTP, HTTPS, etc.) connections:

```bash
$ sudo ufw allow ssh
$ sudo ufw allow 1883/tcp

$ sudo ufw allow http
$ sudo ufw allow https
$ sudo ufw allow 8080/tcp
$ sudo ufw allow 8443/tcp
```

We can confirm that ports are open:

```bash
$ sudo ufw show added
Added user rules (see 'ufw status' for running firewall):
ufw allow 22/tcp
ufw allow 80/tcp
ufw allow 443
ufw allow 1883/tcp
```

Finally, we are going to enabling UFW service in Debian 12.

Let us see the firewall status:

```bash
$ sudo ufw status
Status: inactive
```

Hence, enable UFW:

```bash
$ sudo ufw enable

$ sudo ufw status
Status: active
To                         Action      From
--                         ------      ----
22/tcp                     ALLOW       Anywhere                  
80/tcp                     ALLOW       Anywhere                  
443                        ALLOW       Anywhere                  
1883/tcp                   ALLOW       Anywhere                  
22/tcp (v6)                ALLOW       Anywhere (v6)             
80/tcp (v6)                ALLOW       Anywhere (v6)             
443 (v6)                   ALLOW       Anywhere (v6)             
1883/tcp (v6)              ALLOW       Anywhere (v6) 
```

We have successfully configured a firewall using UFW on Debian 12. 


## Enable Remote Access to Mosquitto

Starting with the release of Mosquitto version 2.0.0 the default config 
will only bind to `localhost` as a move to a more secure default posture.

```bash
$ sudo vi /etc/mosquitto/mosquitto.conf

# Place your local configuration in /etc/mosquitto/conf.d/
#
# A full description of the configuration file is at
# /usr/share/doc/mosquitto/examples/mosquitto.conf.example

pid_file /run/mosquitto/mosquitto.pid

persistence true
persistence_location /var/lib/mosquitto/

log_dest file /var/log/mosquitto/mosquitto.log

include_dir /etc/mosquitto/conf.d
```

In order to enable remote access for unauthorized clients, the following 
two entries must be made (this of course reduces security):

```bash
listener 1883 0.0.0.0   # !!! enable remote clients 
allow_anonymous true    # !!! enable clients without authentication
```


## References

* [Eclipse Mosquitto](https://mosquitto.org/)

* [How to Install Mosquitto MQTT Message Broker on Debian 11](https://www.howtoforge.com/how-to-install-mosquitto-mqtt-message-broker-on-debian-11/)

* [How To Set Up a Firewall with UFW on Debian 12](https://www.cyberciti.biz/faq/set-up-a-firewall-with-ufw-on-debian-12-linux/)

* [How To Ubuntu Linux Firewall Open Port Command](https://www.cyberciti.biz/faq/how-to-open-firewall-port-on-ubuntu-linux-12-04-14-04-lts/)

_Egon Teiniker, 2025-2026, GPL v3.0_