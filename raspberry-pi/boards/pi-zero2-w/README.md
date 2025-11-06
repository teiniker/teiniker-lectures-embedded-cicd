# Raspberry Pi Zero 2 W

At the heart of Raspberry Pi Zero 2 W is **RP3A0**, a custom-built system-in-package designed by Raspberry Pi in the UK. 

* 1GHz quad-core 64-bit Arm Cortex-A53 CPU
* 512MB SDRAM
* 2.4GHz 802.11 b/g/n wireless LAN
* Bluetooth 4.2, Bluetooth Low Energy (BLE), onboard antenna
* Mini HDMI® port and micro USB On-The-Go (OTG) por
* microSD card slot
* CSI-2 camera connector
* HAT-compatible 40-pin header footprint (unpopulated)
* Micro USB power (5V/2.5A)
* Size: 65mm × 30mm

We also need: 
* microSD card 
* 12.5W (5V/2.5A) power supply (microUSB)
* miniHDMI to HDMI adapter 


## Setup 

Because of the limited resources, we setup a headless version of Raspberry Pi OS.
In the **Raspberry Imager** select **Raspberry Pi OS Lite (64-bit or 32-bit)**.

* Settings: 
    - Enable SSH (For Remote Access)
    - Set Up Wi-Fi 

* Insert the microSD card into the Raspberry Pi. 

* Connect power and let it boot.	

* Connect to the Raspberry Pi using SSH:
    ```Bash
    ping raspberrypi.local
    ssh pi@raspberrypi.local (Default password: raspberry)
    ```

    Using `raspberrypi.local` works because the Raspberry Pi advertises 
    its hostname on your local network using **multicast DNS (mDNS)**:

    * When your Raspberry Pi boots up, it runs a service (typically Avahi 
        on Linux) that announces its hostname (e.g., raspberrypi) using mDNS. 
        This protocol lets devices on the same local network resolve hostnames 
        without needing a central DNS server.

    * The `.local` suffix is a convention used for mDNS. When you type 
        raspberrypi.local on your PC, your system sends out a multicast 
        query asking, "Who has the hostname 'raspberrypi'?" The Raspberry 
        Pi responds with its IP address.



## References

* [Raspberry Pi Zero 2 W](https://www.raspberrypi.com/products/raspberry-pi-zero-2-w/)
