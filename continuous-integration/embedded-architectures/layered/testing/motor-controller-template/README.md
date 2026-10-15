# Dependency Injection: Interface vs. Template

To test a `MotorController` without real hardware, we must replace the
CAN bus driver by a **test double** (mock object). This is only possible
if the controller does not create the driver itself, but gets it from
outside (**Dependency Injection**).

In C++ there are two ways to make a dependency replaceable:

* **Runtime polymorphism**: an abstract base class with virtual functions
    (see `../motor-controller`).

* **Compile-time polymorphism**: a template class with a type parameter
    (this project).

Both versions implement the same controller logic and are tested with the
same test cases.


## Runtime Polymorphism (Interface)

The dependency is described by an **abstract base class** with pure
virtual functions. The real driver and the mock both inherit from it.

```C++
class CanInterface
{
public:
    virtual ~CanInterface(void) = default;

    virtual void sendMessage(const CanMessage& message) = 0;
    virtual CanMessage receiveMessage(void) = 0;
};
```

The controller only knows the interface and stores a `shared_ptr` to it:

```C++
class MotorController
{
private:
    std::shared_ptr<CanInterface> _can;
    uint8_t _current_speed;

public:
    MotorController(std::shared_ptr<CanInterface> can)
        : _can(can), _current_speed(0) {}
    // ...
};
```

The mock implements the interface, so every method needs `override`:

```C++
class MockCan : public CanInterface
{
public:
    MOCK_METHOD(void, sendMessage, (const CanMessage& message), (override));
    MOCK_METHOD(CanMessage, receiveMessage, (), (override));
};
```

In the test, the mock is created on the heap and shared with the
controller:

```C++
auto mock = std::make_shared<MockCan>();
MotorController controller(mock);

EXPECT_CALL(*mock, receiveMessage()).WillOnce(Return(speed_msg));
```

Which implementation is called is decided **at runtime** via the
**vtable** of the object.


## Compile-time Polymorphism (Template)

There is **no base class**. The controller is a template class, and the
type of the CAN bus is a **type parameter**:

```C++
template<typename CanBus>
class MotorController
{
private:
    CanBus& _can;
    uint8_t _current_speed;

public:
    explicit MotorController(CanBus& can) : _can(can), _current_speed(0) {}
    // ...
};
```

The interface is only **implicit**: every type used for `CanBus` must
provide the methods called by the controller:

```C++
void sendMessage(const CanMessage& message);
CanMessage receiveMessage(void);
```

The real driver is a normal class without virtual functions:

```C++
class CanBus
{
public:
    void sendMessage(const CanMessage& message);
    CanMessage receiveMessage(void);
};
```

The mock has the same methods, but no base class and no `override`
(see "Mocking Non-virtual Methods" in the gMock Cookbook):

```C++
class MockCan
{
public:
    MOCK_METHOD(void, sendMessage, (const CanMessage& message));
    MOCK_METHOD(CanMessage, receiveMessage, ());
};
```

We decide **at compile time** which class is used, by creating different
instances of the template:

```C++
// Production code (src/main.cpp)
CanBus bus;
MotorController<CanBus> controller(bus);

// Test code (test/test.cpp)
MockCan mock;
MotorController<MockCan> controller(mock);

EXPECT_CALL(mock, receiveMessage()).WillOnce(Return(speed_msg));
```

The compiler generates two separate classes, `MotorController<CanBus>`
and `MotorController<MockCan>`. Since the type is known, method calls are
resolved statically and **can be inlined**.

Because the compiler needs the full code to instantiate a template, the
whole implementation of `MotorController` is in the header file
`include/motor_controller.h`. There is no `motor_controller.cpp`.


## Reference vs. `shared_ptr`

In the template version, the controller holds a **reference** to the bus:

* **Ownership**: the controller only uses the bus, it does not own it.
    On an embedded target, the driver is typically a static object which
    lives as long as the program runs.

* **No heap**: `std::make_shared` allocates memory on the heap, which is
    often avoided or forbidden in embedded software (MISRA C++,
    AUTOSAR C++14).

* **No overhead**: no control block and no atomic reference counter.

* **Never null**: a reference must be bound in the constructor.

The price is that **the bus must outlive the controller**, otherwise the
reference dangles. In the tests this is guaranteed, because `mock` is
defined before `controller` and destroyed after it.

A `shared_ptr` makes sense when objects are created dynamically (e.g. by
a factory at runtime) and several components share them without a clear
owner.


## Comparison

| Aspect                  | Interface (virtual)     | Template                |
|-------------------------|-------------------------|-------------------------|
| Binding                 | at runtime (vtable)     | at compile time         |
| Interface               | explicit (base class)   | implicit (used methods) |
| Call overhead           | indirect call           | direct call, inlining   |
| Memory                  | vtable pointer per obj. | none                    |
| Dependency held as      | `shared_ptr` (heap)     | reference (no heap)     |
| Implementation          | `.cpp` file             | header file             |
| Code size               | one class               | one class per instance  |
| Exchange at runtime     | possible                | not possible            |
| Compiler errors         | easy to read            | often hard to read      |
| Mock class              | inherits, `override`    | no base class           |

**Interface** based design is more flexible and keeps the implementation
in `.cpp` files. It is the better choice when the implementation must be
selected at runtime (e.g. configuration, plugins).

**Template** based design has no runtime overhead and needs no heap.
It is a good choice for embedded systems, where the hardware driver is
fixed at compile time and the only reason for the abstraction is
**testability**.


## Build and Run

```bash
cmake -S . -B build
cmake --build build
./build/test/test      # Tests using MotorController<MockCan>
./build/src/main       # Production code using MotorController<CanBus>
```


## References

* [gMock Cookbook: Mocking Non-virtual Methods](https://google.github.io/googletest/gmock_cook_book.html#MockingNonVirtualMethods)


_Egon Teiniker, 2025-2026, GPL v3.0_
