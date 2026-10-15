# Template-Based Hardware Abstraction Layer (HAL)

## C++ Templates 

A **template** is a blueprint from which the **compiler generates code**.
Instead of writing the same function or class for every data type (or every
target platform), we write it once and leave some parts open as
**template parameters**. When the template is used with concrete arguments,
the compiler creates a concrete function or class from it. This process is
called **template instantiation** and happens entirely at **compile time**.

See: [GitHub: C++ Repo - Templates](https://github.com/teiniker/teiniker-lectures-embedded-cxx/tree/main/programming-c%2B%2B/templates)

Template parameters can be:

* **Type parameters** (`typename T`): a type, e.g. `int16_t`, `float`, or
    a class describing a target platform.

* **Non-type parameters** (`std::size_t N`): a compile-time constant value,
    e.g. a buffer size, a pin number, or a register address.

* **Template template parameters**: a template itself (rarely needed).


### Function Templates

A **function template** defines a family of functions.

_Example_: Limit a value to a given range

```C++
template <typename T>
T clamp(T value, T low, T high)
{
	return (value < low) ? low : (value > high) ? high : value;
}
```

The template arguments can be specified **explicitly** or **deduced** by the
compiler from the function arguments:

```C++
int16_t temperature = clamp<int16_t>(raw, -40, 125);	// T = int16_t
float voltage = clamp(3.3f, 0.0f, 5.0f);	// T = float (deduced)
```

Deduction is strict: no implicit conversions are applied. Calling
`clamp(raw, -40, 125)` with an `int16_t` variable `raw` fails, because the
literals are of type `int`:

```
error: no matching function for call to 'clamp(int16_t&, int, int)'
note:   deduced conflicting types for parameter 'T' ('short int' and 'int')
```


### Class Templates

A **class template** defines a family of classes. Template parameters can be
used everywhere in the class: as types of member variables, in method
signatures, and as constants.

_Example_: A ring buffer (e.g. for received UART bytes) without any heap
allocation

```C++
template <typename T, std::size_t N>
class RingBuffer
{
	static_assert((N & (N - 1)) == 0, "N must be a power of two");

	private:
		std::array<T, N> _buffer{};
		std::size_t _head = 0;
		std::size_t _tail = 0;

	public:
		bool empty(void) const { return _head == _tail; }
		bool full(void) const { return _head - _tail == N; }

		bool push(const T& value)
		{
			if (full()) return false;
			_buffer[_head++ % N] = value;
			return true;
		}

		bool pop(T& value)
		{
			if (empty()) return false;
			value = _buffer[_tail++ % N];
			return true;
		}
};
```

For class templates, the template arguments are usually given explicitly.
Each combination of arguments is a **different type**:

```C++
RingBuffer<uint8_t, 64> rx_buffer;	// 64 bytes, UART receive buffer
RingBuffer<Sample, 16> adc_samples;	// 16 ADC samples
```

The `static_assert` is checked for every instantiation: `RingBuffer<uint8_t,
100>` causes a **compile error**, not a runtime error.


### Template Instantiation

Templates have some properties that differ from ordinary code:

* **Code is generated on demand**: Only the template arguments that are
    actually used are instantiated, and for class templates only the member
    functions that are actually called.

* **The definition must be visible**: The compiler needs the complete
    template code wherever it is instantiated. Therefore, templates are
    usually implemented in **header files** (header-only). Alternatively,
    a `.cpp` file can **explicitly instantiate** a fixed set of variants
    (`template class EEPROM<Atmel>;`).

* **Two-phase checking**: Code that does not depend on template parameters
    is checked when the template is defined. Code that depends on template
    parameters (e.g. `Target::vendor`) is only checked when the template is
    instantiated.

The [EEPROM example](../eeprom/README.md) shows these properties in detail.


## Using C++ Templates in Embedded Systems

Embedded systems have limited memory and processing power, and many errors
are hard to find on the target. Templates help in several ways:

* **Zero-overhead abstraction**: All decisions are made at compile time.
    There are no virtual function tables and no indirect calls, and the
    compiler can inline and optimize the generated code as if it were
    written by hand.

* **No dynamic memory**: Sizes passed as non-type parameters are known at
    compile time, so buffers can be allocated statically (`std::array`)
    instead of on the heap. The memory usage is visible in the map file.

* **Compile-time checks**: `static_assert` and (since C++20) **concepts**
    detect invalid configurations, e.g. a wrong buffer size or a pin number
    that does not exist, during the build instead of on the target.

* **Type safety**: Unlike macros or `void*` interfaces, templates are fully
    type-checked. Mixing up e.g. two different GPIO pins or peripherals
    becomes a type error.

* **Reuse without duplication**: The same driver code can be used for
    different targets, data types, or buffer sizes without copy and paste.

* **Testability**: The same code can be instantiated with real hardware
    access on the target and with fakes or mocks on the host, all in one
    build (see below).

These benefits come at a cost:

* **Code size**: Every instantiation generates its own code. Many different
    template arguments can increase the size of the firmware (code bloat).

* **Build time**: Header-only templates are compiled again in every
    translation unit that includes them.

* **Error messages**: Errors inside templates can lead to long and hard to
    read compiler messages. Concepts make them considerably better.

* **Complexity**: Advanced template techniques are hard to read and debug
    for developers without template experience.


## Using C++ Templates to Build an HAL 

In a template-based HAL, the **hardware-dependent parts become template
parameters**. The selection of the hardware happens in the source code, by
choosing template arguments, usually in one central configuration header.

### Target Traits (Policy Classes)

The differences between targets are described by small classes that are
passed as template arguments. They contain constants (**traits**) or
functions (**policies**) for one specific target.

```C++
struct Atmel     { static constexpr const char* vendor = "Atmel"; };
struct Microchip { static constexpr const char* vendor = "Microchip"; };

template <typename Target, std::size_t Size = 1024*8>
class EEPROM 
{
	// ...
	std::string vendor(void) const { return Target::vendor; }
};

using Eeprom = EEPROM<Microchip>;	// central target selection
```

This approach replaces conditional compilation (see the
[EEPROM example](../eeprom/README.md)).


### Peripherals as Template Parameters

Register addresses and pin numbers are known at compile time, so they can be
non-type template parameters. Every pin becomes its own type, invalid
pins are rejected by the compiler, and all calls compile to direct register
accesses.

_Example_: GPIO pin of an STM32F4 microcontroller

```C++
template <std::uintptr_t Base, uint8_t Pin>
struct GpioPin
{
	static_assert(Pin < 16, "A GPIO port has only 16 pins");

	static volatile uint32_t& bsrr(void)
	{
		return *reinterpret_cast<volatile uint32_t*>(Base + 0x18);
	}

	static void set(void)   { bsrr() = (1u << Pin); }
	static void clear(void) { bsrr() = (1u << (Pin + 16)); }
};

using Led = GpioPin<0x40020000, 5>;	// GPIOA, pin 5

Led::set();
```

With optimization enabled, `Led::set()` compiles to a **single store
instruction**, exactly as if we had written to the register directly.


### Static Dependency Injection

A driver for an external device (e.g. an I2C EEPROM) does not access the
hardware directly but uses a bus driver. If the bus type is a template
parameter, the bus can be **injected at compile time**:

```C++
template <typename Bus>
class Eeprom24C02
{
	private:
		static constexpr uint8_t DEVICE = 0x50;	// I2C device address
		Bus& _bus;

	public:
		explicit Eeprom24C02(Bus& bus) : _bus(bus) {}

		uint8_t read(const uint8_t address)
		{
			_bus.write(DEVICE, address);	// set address pointer
			return _bus.read(DEVICE);
		}
};
```

On the target, we use the real I2C driver. On the host, we use a mock
object. Because there is no common base class, the mock methods do **not**
have to be virtual (GoogleMock calls this "mocking non-virtual methods"):

```C++
class MockI2c
{
	public:
		MOCK_METHOD(void, write, (uint8_t device, uint8_t data));
		MOCK_METHOD(uint8_t, read, (uint8_t device));
};

TEST(Eeprom24C02Test, Read)
{
	MockI2c bus;
	Eeprom24C02<MockI2c> eeprom(bus);

	EXPECT_CALL(bus, write(0x50, 0x10));
	EXPECT_CALL(bus, read(0x50)).WillOnce(testing::Return(0xaa));

	EXPECT_EQ(0xaa, eeprom.read(0x10));
}
```

This is the template-based counterpart of dependency injection with
interfaces, without the run-time overhead of virtual functions.


## Comparison of HAL Implementation Techniques

We know four ways to select a hardware-specific implementation:

* **Conditional compilation**: The pre-processor removes the code of all
    other targets (`#ifdef`). Selection by macros (`-DTARGET_1`).

* **Link-time polymorphism**: The same header is implemented by different
    `.cpp` files. The build system decides which object files are linked.

* **Templates (compile-time polymorphism)**: The target is a template
    argument. Selection in the source code, usually via a type alias.

* **Run-time polymorphism**: An abstract base class with virtual functions
    defines the interface, the implementation is chosen when objects are
    created and called via the virtual function table.

|                        | `#ifdef`  | Linker    | Templates   | Virtual    |
|------------------------|-----------|-----------|-------------|------------|
| Binding time           | Pre-proc. | Link time | Compile     | Run time   |
| Selected by            | Macros    | CMake     | Source code | Objects    |
| Run-time overhead      | None      | None      | None        | V-table    |
| Inlining possible      | Yes       | Limited   | Yes         | Rarely     |
| Variants in one build  | No        | No        | Yes         | Yes        |
| All variants compiled  | No        | Per build | If used     | Yes        |
| Mocking in unit tests  | No        | Fake impl | Yes         | Yes        |
| Works in C             | Yes       | Yes       | No          | Manually   |
| Code size              | Minimal   | Minimal   | Per variant | Overhead   |
| Readability            | Poor      | Good      | Medium      | Good       |

Notes on the table:

* **Inlining** with link-time polymorphism requires link-time optimization
    (LTO), because the implementation is in a different translation unit.

* **All variants compiled**: For templates, only the variants and member
    functions that are actually used are compiled. Explicit instantiation in
    the tests (`template class EEPROM<Atmel>;`) forces the compiler to check
    all of them in the CI build.

* **Works in C**: C has no virtual functions, but run-time polymorphism can
    be implemented manually with structs of function pointers.

**Recommendation**:

* Use **conditional compilation** only for small, local differences.

* Use **link-time polymorphism** for larger implementations that differ
    completely between targets, or if the code must also work in C.

* Use **templates** if the hardware is known at compile time and both
    efficiency and testability matter, e.g. for low-level drivers and
    register access.

* Use **run-time polymorphism** if the implementation must be selected at
    run time (e.g. different display types detected on startup) or if the
    small overhead of virtual functions is acceptable.

In practice, these techniques are combined: e.g. templates for register
access in the lower HAL layers, and interfaces with virtual functions in the
upper layers, where flexibility is more important than a few clock cycles.


## References

* [Designing a HAL in C++](https://blog.mbedded.ninja/programming/languages/c-plus-plus/designing-a-hal-in-cpp/)

* [GitHub: C++ Repo - Templates](https://github.com/teiniker/teiniker-lectures-embedded-cxx/tree/main/programming-c%2B%2B/templates)

* [GoogleMock: Mocking Non-virtual Methods](https://google.github.io/googletest/gmock_cook_book.html#MockingNonVirtualMethods)


_Egon Teiniker, 2025-2026, GPL v3.0_
