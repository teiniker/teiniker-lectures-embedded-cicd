# pigpio Library


This is a C library for the Raspberry which allows control of the General Purpose Input Outputs (GPIO).

* Sampling and time-stamping of GPIO 0-31 between 100,000 and 1,000,000 times per second
* Provision of PWM on any number of the user GPIO simultaneously
* Provision of servo pulses on any number of the user GPIO simultaneously
* Callbacks when any of GPIO 0-31 change state (callbacks receive the time of the event accurate to a few microseconds)
* Notifications via pipe when any of GPIO 0-31 change state
* Callbacks at timed intervals
* Reading/writing all of the GPIO in a bank (0-31, 32-53) as a single operation
* Individually setting GPIO modes, reading and writing
* Socket and pipe interfaces for the bulk of the functionality in addition to the underlying C library calls
* Construction of arbitrary waveforms to give precise timing of output GPIO level changes (accurate to a few microseconds)
* Software serial links, I2C, and SPI using any user GPIO
* Rudimentary permission control through the socket and pipe interfaces so users can be prevented from "updating" inappropriate GPIO
* Creating and running scripts on the pigpio daemon



## References

* [The pigpio library](https://abyz.me.uk/rpi/pigpio/)
* [GitHub: pigpio](https://github.com/joan2937/pigpio)

* Dogan Ibrahim. **C Programming on Raspberry Pi**. Elektor 2021