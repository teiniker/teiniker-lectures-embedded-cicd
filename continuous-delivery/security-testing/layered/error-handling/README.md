# Error Handling in Embedded Systems

In input validation, error handling plays a vital role.
When invalid input is detected, an error must be reported.


## Exception Handling

If a supplied value is invalid, a `std::invalid_argument` exception
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

* Amar Mahmutbegović. **C++ in Embedded Systems: A practical transition from C to modern C++**. Packt Publishing, 2025

_Egon Teiniker, 2025, GPL v3.0_
