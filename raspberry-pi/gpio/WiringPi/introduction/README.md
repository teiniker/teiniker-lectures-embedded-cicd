# WiringPi Library

This library is written in C and is designed to provide fast and efficient control 
of the GPIO pins by directly accessing the hardware registers using DMA.

WiringPi supports all Raspberry Pi Boards including Pi 5

WiringPi is widely used in numerous projects, making it a reliable choice for your Raspberry Pi GPIO needs.


## Interaction wit Kernel  

WiringPi is a **user-space library** that talks to Raspberry Pi hardware using a mix 
of memory-mapped I/O (MMIO) for fast access to GPIO registers and some kernel interfaces 
(e.g., `/dev/mem`, `/dev/gpiomem`, `/sys/class/gpio`, `/dev/spidev*`, `/dev/i2c-*`) 
depending on what features we use.

* User-space library, not a kernel driver
    - WiringPi does not run in the kernel and does not require custom kernel modules.
    - Our program simply links against **libwiringPi.so** and makes syscalls like any user program.
    - All hardware control happens from user space using Linux-exposed interfaces.

* GPIO register access through /dev/gpiomem
    - WiringPi uses `/dev/gpiomem` to get memory-mapped access to the SoC’s GPIO peripheral registers.
    - `/dev/gpiomem` allows GPIO access without full root privileges.
    - The library `mmap()`s this device so it can read/write registers directly in RAM space.

* SPI and I2C via Linux device files: 
    - If we use WiringPi’s SPI/I2C helpers:
        - SPI: `/dev/spidev*` (spidev kernel driver)
        - I2C: `/dev/i2c-*` (i2c-dev kernel driver)

* Interrupts via kernel GPIO edge detection
    - For edge interrupts (e.g., `wiringPiISR()`):
        - It uses the kernel’s epoll / poll mechanism on a GPIO sysfs or character device.
        - The kernel detects the edge; WiringPi gets notified in user space.
        - No busy-looping required; very lightweight.


## Operations 

* Initialization: At the beginning, the WiringPi Library must be initialized. 



## Setup 

Install library from Source:

```bash
# fetch the source
$ sudo apt install git
$ git clone https://github.com/WiringPi/WiringPi.git
$ cd WiringPi

# build the package
$ ./build debian
$ mv debian-template/wiringpi_3.16_arm64.deb .

# install it
$ sudo apt install ./wiringpi_3.16_arm64.deb 
```


## References

* [WiringPi Library](https://github.com/WiringPi/WiringPi)

* [WiringPi functions documentation](https://github.com/WiringPi/WiringPi/blob/master/documentation/english/functions.md)

_Egon Teiniker, 2025, GPL v3.0_