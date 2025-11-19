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


## Exception Handling
If the supplied value is invalid, a `std::invalid_argument` exception
is thrown and object creation is prevented.

_Example:_ Input validation using `std::invalid_argument` exception

```c++
Percentage Percentage::make(int value) 
{
    if (value < 0 || value > 100) 
    {
        throw std::invalid_argument("Invalid percentage value");
    }
    return Percentage(value);
}
```

In small embedded systems, exceptions tend to be more trouble than they're 
worth. They add **significant code size** because the compiler has to pull 
in stack unwinding machinery, type info, and runtime handlers. 

They also make **timing unpredictable**, since throwing and catching an 
exception isn’t constant-time — a big no-go in real-time code. 

And when RAM and flash are tight, the extra metadata and runtime support 
can eat up resources fast. 

Because of all that, **most embedded teams just turn them off**:

* `-fno-exceptions`: Turn off exceptions
* `-fno-rtti`: Turn off RTTI (Run-Time Type Information)

_Example:_ Global configuration in CMake 

```cmake
add_compile_options(-fno-exceptions -fno-rtti)
```

Once you build with these flags:

* `throw` and `catch` are forbidden
* `typeid()` and `dynamic_cast` stop working for polymorphic types
* The compiler no longer links in the big exception / RTTI runtime baggage


## Using std::optional 

Many embedded developers use **explicit error handling** instead.

`std::optional` basically a tiny container that can hold:
* a **value of some type T**, or
* **no value** at all

`std::optional<T>` behaves like:
* A **boolean**: tells us whether a value exists
* A **box**: if it exists, we get the value inside

`std::optional` gives us a clean, type-safe alternative to exceptions.

_Example:_ Input validation using `std::optional`

```c++
#include <optional>

std::optional<int> x;

if (x) 
{
    // x contains a value
    int v = x.value();
} 
else 
{
    // nothing there
}
```


## References

* [Value Object — Martin Fowler](https://martinfowler.com/bliki/ValueObject.html)

* Eric Evans. **Domain-Driven Design: Tackling Complexity in the Heart of Software**. Pearson International, 2003.


_Egon Teiniker, 2025, GPL v3.0_
