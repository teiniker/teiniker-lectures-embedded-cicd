# Value Object Pattern 

The Value Object pattern is a design pattern for creating immutable objects 
whose identity is based on their attributes' values rather than a unique reference. 

Key characteristics:

* **Immutability**: Once a Value Object is created, its internal state cannot 
be changed. Any operation that would appear to modify it must instead return 
a new instance of the object.

* **Value-based equality**: Two Value Objects are considered equal if all their 
attributes have the same values, regardless of whether they are different objects 
in memory. This requires overriding equality methods and potentially operators.

* **No conceptual identity**: Unlike entities, Value Objects don't have a unique 
ID. They represent a descriptive aspect of the domain that is identified by its 
state.

* **Encapsulates data and logic**: They can wrap primitive types, **adding validation** 
and business rules to **prevent invalid states**, solving the problem of "primitive obsession". 


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

Wenn der Übergebene Wert nicht valide ist, wird eine `std::invalid_argument`
Exception geworfen und die Erstellung des Objekts verhindert.

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


_Egon Teiniker, 2025, GPL v3.0_