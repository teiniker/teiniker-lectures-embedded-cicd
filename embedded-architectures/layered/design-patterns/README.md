# Design Patterns in Layered Architectures


The following design patterns help to implement a layered architecture:


## Creational Patterns 

* [Factory Method](https://github.com/teiniker/teiniker-lectures-embedded-softwaredesign/tree/master/design-patterns/creational/factory-method): 
    This pattern defines an interface for creating an 
    object but lets subclasses alter the type of objects that will be 
    created. It encapsulates the object creation process and delegates 
    it to subclasses.

* [Dependency Injection]():
    This pattern supplies an object’s required dependencies from the 
    outside rather than having the object create them itself, promoting 
    loose coupling and easier testing.


## Structural Patterns

* [Facade](https://github.com/teiniker/teiniker-lectures-embedded-softwaredesign/tree/master/design-patterns/structural/facade):
    This pattern provides a simplified interface to a complex subsystem, 
    making it easier to use. It defines a higher-level interface that 
    makes the subsystem easier to use.

* [Adapter](adapter/):
    This pattern converts the interface of a class into another interface 
    that clients expect, allowing otherwise incompatible classes to work 
    together.


## Behavioral Patterns

* [Strategy](https://github.com/teiniker/teiniker-lectures-embedded-softwaredesign/tree/master/design-patterns/behavioral/strategy):
    This pattern defines a family of algorithms, encapsulates each one, 
    and makes them interchangeable. It lets the algorithm vary independently 
    from the clients that use it.


* [Template Method](https://github.com/teiniker/teiniker-lectures-embedded-softwaredesign/tree/master/design-patterns/behavioral/template-method):
    This pattern defines the skeleton of an algorithm in an operation, 
    deferring some steps to subclasses. It lets subclasses redefine certain 
    steps of an algorithm without changing its structure.


## References

* E. Gamma, R. Helm, R. Johnson, J. Vlissides. **Design Patterns, Elements of Reusable Object-Oriented Software**. Addison-Wesley, 1995

_Egon Teiniker, 2025, GPL v3.0_