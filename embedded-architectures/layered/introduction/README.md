# Layered Architecture 

In embedded systems, a layered architecture is a design approach that organizes 
software into **distinct layers**, each with specific roles and well-defined 
interfaces. 

A layered architecture divides the software stack into **hierarchical layers**, where:
* **Each layer provides services** to the layer above it.
* **Each layer uses services** from the layer below it.
* **Direct access across non-adjacent layers is avoided**, ensuring abstraction and 
    separation of concerns.

This allows developers to modify one layer (e.g., hardware drivers) without 
affecting the higher-level application logic.


## Layers in an Embedded System


* **Hardware Layer**: The physical components of the system.

    _Examples:_ Microcontroller, sensors, actuators, communication interfaces.

* **Hardware Abstraction Layer (HAL)**: Provides a consistent API to interact with 
    hardware, hiding device-specific details.

    _Examples:_ GPIO, ADC, UART drivers

* **Operating System (or RTOS) Layer**: Manages tasks, timing, and system resources.

    _Examples:_ FreeRTOS, Zephyr

* **Middleware Layer**: Offers reusable software components and communication services.

    _Examples:_ TCP/IP stack, file system, Bluetooth stack

* **Application Layer**: Implements the specific logic and behavior of the system.

    _Examples:_ Control algorithms, user interface, application logic



## Implementation Techniques 

To implement a layered architecture, developers can leverage both language-specific 
mechanisms and language-agnostic design patterns. 

* **Language mechanisms** include features such as modules, namespaces, interfaces, 
    and access control (e.g., `private`, `protected`, `public`), which help enforce 
    boundaries between layers.

* **Design patterns** such as the Facade, Adapter, and Dependency Injection can further 
    decouple layers, promote abstraction, and facilitate communication between them.

By combining these techniques, it is possible to achieve a clear separation of concerns, 
improve code maintainability, and ensure that each layer interacts only through well-defined 
interfaces.









## References

* Robert C. Martin. **Clean Architecture: A Craftsman's Guide to Software Structure and Design**. Addison-Wesley, 2017



_Egon Teiniker, 2025, GPL v3.0_