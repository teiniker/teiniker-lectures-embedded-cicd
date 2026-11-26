# Value Object Pattern 

The Value Object Pattern is a design pattern used in the domain of 
domain-driven design. 

Here are the key aspects of this pattern:

* **Immutability**: Value objects are immutable, meaning once they 
    are created, their state cannot be changed. This makes them 
    inherently safe for use in a concurrent environment since they 
    can't be modified once they've been instantiated.

* **Identity-less**: Unlike entities, value objects do not have a unique 
    identity. Two value objects are considered equal if all their fields 
    are equal. Their identity is solely based on their attributes, not 
    on a unique identifier.

* **Self-validation**: Value objects should validate their own state 
    during creation. This ensures they are always in a valid state once 
    they are instantiated.

* **Simplicity and Focus**: They typically have a small number of attributes 
    and are focused on modeling a specific aspect of the domain. 
    For example, an `Address` value object might include fields for street, 
    city, and postal code, and nothing more.

* **Lifecycle**: They are often created, used for a specific calculation or 
    operation, and then discarded. They don't usually have a long lifecycle 
    within the application.

The Value Object pattern can be effectively used in the context of software 
security, particularly for **input validation**.

By using value objects for input validation, we ensure **consistency** throughout 
the application. Instead of scattering validation logic across the application, 
it is centralized within the value object, making it easier to maintain and update 
the validation rules as needed.


## Implement a Value Object in C++ 

Implementing a Value Object in C++, we need:
* **Immutable value** with a well-defined range or format.
* **Getter method** for the stored value.
* **Static creation method** that **validates input** and returns an instance.
* **Private constructor** to enforce use of the creation method.
* **Copy constructor** and **assignment operator** (defaulted when suitable).
* **Spaceship operator** (operator<=>) for comparisons.


_Example:_ Percentage Value Object  

```c++
class Percentage 
{
public:
    // Creation method
    static std::optional<Percentage> make(int value);

    int value() const { return value_;  }   
    
    // Copy constructor and assignment operator
    Percentage(const Percentage&) = default;

    // Assignment operator
    Percentage& operator=(const Percentage&) = default;

    // Comparison: gives us ==, !=, <, <=, >, >= for free, all by comparing the object’s members.
    auto operator<=>(const Percentage&) const = default;

private:
    // private constructor to enforce use of creation method
    explicit Percentage(int v) : value_(v) {}
    int value_;
};
```

Most of the time for a simple value object, we can just rely on the 
**implicitly generated copy constructor and assignment operator** 
(or mark them `=default` if you like being explicit) and not write 
custom implementations.

In the static `make()` method, **input validation** happens.
If the passed value is invalid, an **exception is thrown**.
In embedded systems, **return a `std::optional`** instead of throwing.


## References

* [Value Object — Martin Fowler](https://martinfowler.com/bliki/ValueObject.html)

* Eric Evans. **Domain-Driven Design: Tackling Complexity in the Heart of Software**. Pearson International, 2003.


_Egon Teiniker, 2025, GPL v3.0_
