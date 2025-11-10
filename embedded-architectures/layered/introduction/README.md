# Clean Embedded Architecture 

Embedded systems present unique challenges: constrained resources (CPU, 
memory, energy), real-time and timing requirements, close coupling to 
hardware, limited observability and debugging, and long hardware–software 
integration cycles. 

One of the special embedded problems is the **target-hardware bottleneck**. 

When embedded code is structured without applying clean architecture principles 
and practices, we will often face the scenario in which we can **test our code 
only on the target**. 

If the target is the only place where testing is possible, the target-hardware 
bottleneck will slow us down.

Layering comes in many flavors. 


## A Testable Embedded Architecture

The separation between hardware and the rest of the system is given, at least 
once the hardware is defined.
But because of technology advances the hardware will change over time.

A problem is that there is nothing that keeps hardware knowledge from polluting 
all the code so the code will be very hard to change.

**Software and firmware intermingling is an anti-pattern**.


### Hardware Abstraction Layer 

The **line between software and firmware** is typically not so well defined
as the line between code and hardware.
One of our jobs as an embedded software devloper is to firm up that line.

![Embedded Architecture Layers](figures/Embedded-Architecture-Layered.png)

The name of the boundary between the software and the firmware is the 
**Hardware Abstraction Layer (HAL)**.

The HAL exists for the software that sits on top of it, and its API should 
be tailored to that software’s needs. 

_Example:_ The firmware can store bytes and arrays of bytes into flash memory. 
    In contrast, the application needs to store and read name/value pairs to 
    some persistence mechanism. 
    The software should not be concerned that the name/value pairs are stored 
    in flash memory, a spinning disk, the cloud, or core memory. 
    The HAL provides a service, and it does not reveal to the software how it 
    does it. The flash implementation is a detail that should be hidden from 
    software.

A clean embedded architecture’s software is **testable off the target hardware**. 
A successful HAL provides that seam or set of substitution points that facilitate 
off-target testing


### Processor Is a Detail

A clean embedded architecture would use device access registers directly in very 
few places and confine them totally to the firmware.
Anything that knows about these registers becomes firmware and is consequently 
bound to the silicon.

If we use a microcontroller our firmware could isolate these low-level functions 
with some form of a **Processor Abstraction Layer (PAL)**.
Firmware above the PAL could be tested iff-target, making it a little less firm.


### Operating System is a Detail 

What about embedded systems that use a real-time operating system (RTOS) or some
embedded version of Linux?

To give our embedded code a good chance at a long life, we have to treat 
the operating system as a detail and protect against OS dependencies.

A clean embedded architecture isolates software from the operating system,
through an **Operating System Abstraction Layer (OSAL)**.

In some cases, implementing this layer might be as simple as changing the name 
of a function.
In other cases, it might involve wrapping several functions together.

The OSAL can help provide test points so that the valuable application code 
in the software layer can be tested off-target and off-OS.
A clean embedded architecture's software is **testable off the target operating system**.


### Programming to Interfaces and Substitutability

The idea of a layered architecture is build on the idea of **programming to interfaces**.

When one module interacts with another though an interface, we can substitute 
one service provider for another.

One basic rule of thumb is to **use header files as interface definitions**.
Limit header file contents to functiondeclarations as well as the constants 
and struct names that are needed by the function.

Don't clutter the interface header files with data structures, constants, and typedefs
that are needed by only the implementation.

A clean embedded architecture is testable within the layers because 
modules interact through interfaces.
Each interface provides a substitution point that facilitates off-target testing.



### Implementation Techniques 

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
    - Chapter 29: Clean Embedded Architecture 


_Egon Teiniker, 2025, GPL v3.0_