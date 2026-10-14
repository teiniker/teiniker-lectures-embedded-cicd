# Example: Display

This example models two character displays with the same interface:

* **LCD**: a 16x2 character LCD, connected via **I2C**.
* **OLED**: a 128x64 OLED (21x8 characters with a 6x8 font), connected via
    **SPI**.

In the [linker example](../../linker/display/README.md), the two variants
are two `.cpp` files implementing the same `Display` class, and CMake
selects one of them at link time. Both files contain the same buffer
handling; only the bus access is different.

In this example we use a **class template**: the bus becomes a
**template parameter**, and the display logic exists only once.

```
display/
├── CMakeLists.txt      // Project settings (header-only library)
├── include/
│   ├── display.h       // Class template Display<Bus, Cols, Rows>
│   ├── i2c.h           // Class template I2C<Address, SDA, SCL>
│   ├── spi.h           // Class template SPI<MOSI, MISO, SCK, CS>
│   ├── lcd.h           // Type alias LCD
│   └── oled.h          // Type alias OLED
└── test/
    ├── mock_bus.h      // Test double for the bus
    └── test.cpp        // GoogleTest unit tests (LCD, OLED, mock)
```


## Bus Classes

Each bus is a class template with **non-type parameters**: the I2C address
and the pin numbers are compile-time constants.

_Example_: `include/i2c.h`

```C++
template <uint8_t Address, uint8_t SDA, uint8_t SCL>
class I2C
{
	public:
		static constexpr uint8_t address = Address;
		static constexpr uint8_t sda = SDA;
		static constexpr uint8_t scl = SCL;

		void init(void)
		{
			// Initialize I2C communication
		}

		void write(const uint8_t data)
		{
			// Write data to I2C
		}
};
```

`I2C<0x27, 21, 22>` and `I2C<0x3C, 21, 22>` are two **different types**.
The configuration does not need any RAM, and the compiler can use the
constants directly in the generated code.

`SPI<MOSI, MISO, SCK, CS>` in `include/spi.h` is built the same way.
`I2C` and `SPI` have no common base class: they only have to provide the
same member functions.


## Class Template

The `Display` class gets three template parameters:

```C++
template <typename Bus, std::size_t Cols, std::size_t Rows>
class Display
{
	static_assert(Cols > 0 && Rows > 0, "Display size must not be zero");

	private:
		Bus _bus;
		std::array<char, Cols * Rows + 1> _buffer; // +1: terminating '\0'
		std::size_t _index;

	public:
		Display(void) : _buffer{}, _index{0}
		{
			_bus.init();
		}

		void print(const char c)
		{
			if(_index < Cols * Rows)	// ignore characters beyond the display
			{
				_buffer[_index++] = c;
				_bus.write(static_cast<uint8_t>(c));
			}
		}
	// ...
};
```

* `typename Bus`: A **type parameter**, the bus used to talk to the
    hardware. The display stores a bus object as a member and calls
    `_bus.init()` and `_bus.write()`.

* `std::size_t Cols, Rows`: **Non-type parameters** for the display size.
    Because the size is known at compile time, the buffer is a `std::array`
    instead of `new[]`/`delete[]`: no heap allocation, no destructor, and
    copying a `Display` is safe.

* `static_assert`: Invalid sizes are rejected by the **compiler**, e.g.
    `Display<MockBus, 0, 2>`:
    ```
    error: static assertion failed: Display size must not be zero
    ```

The buffer has one extra element which is never written by `print()`, so
`buffer()` always returns a null-terminated string. Characters beyond the
display size are ignored instead of overwriting memory.


## Requirements on the Bus

The template does not say which types are allowed for `Bus`. Any type works
as long as the code in `Display` compiles with it. This is called an
**implicit interface** (or **duck typing** at compile time):

```C++
void init(void);
void write(uint8_t data);
```

If a type does not provide these member functions, the error is reported
when the template is **instantiated**:

```
error: 'struct Foo' has no member named 'write'
```

With C++20, these requirements can be written down explicitly as a
**concept**, which leads to shorter error messages.


## Selecting the Variant

The two variants are defined by **type aliases**, one per header file.
They replace the `src-lcd/` and `src-oled/` directories of the linker
example.

_Example_: `include/lcd.h` and `include/oled.h`

```C++
using LCD  = Display<I2C<0x27, 21, 22>, 16, 2>;
using OLED = Display<SPI<10, 9, 11, 8>, 21, 8>;
```

The application code only uses `LCD` or `OLED`:

```C++
LCD display;
display.print('H');
```

There is no `USE_IMPL` option in `CMakeLists.txt` anymore. Since there is
nothing to compile, the project is a **header-only library**.

```CMake
# Header-only library: nothing to compile, only usage requirements
add_library(display INTERFACE)
target_include_directories(display INTERFACE ${PROJECT_SOURCE_DIR}/include)
```


## Testing with a Mock Bus

Because the bus is a template parameter, a test can replace the hardware
with a **test double** that records every call:

_Example_: `test/mock_bus.h`

```C++
class MockBus
{
	public:
		bool initialized = false;
		std::vector<uint8_t> written;

		void init(void)
		{
			initialized = true;
		}

		void write(const uint8_t data)
		{
			written.push_back(data);
		}
};
```

The test checks which bytes were actually sent to the bus:

```C++
TEST(DisplayTestGroup, MockBusWriteTest) 
{
    // Setup
    Display<MockBus, 8, 2> display;

    // Exercise
    display.print('H');
    display.print('i');

    // Verify
    vector<uint8_t> expected = {'H', 'i'};
    EXPECT_EQ(expected, display.bus().written);
}
```

In the linker example, this is not possible: the I2C and SPI functions are
part of the same `.cpp` file as the display logic, so a test can only check
the buffer, but not the communication with the hardware.


## Templates vs. Link-Time Polymorphism

* **No duplicated code**: The buffer handling is written once. The variants
    only differ in the bus, which is a separate class.

* **All variants in one build**: LCD, OLED, and the mock bus are compiled
    into the same test executable and tested in a single run.

* **All variants are type-checked**: With link-time selection, the inactive
    `.cpp` file is not compiled at all, so errors there stay hidden.

* **Testability**: The bus can be replaced by a test double without
    changing the production code.

* **Configuration at compile time**: Pins, addresses, and sizes are part
    of the type and can be checked with `static_assert`.

* **Template parameters propagate**: Every function that receives a
    display must either be a template itself or use a type alias like
    `LCD`. `LCD` and `OLED` are different types without a common base class,
    so they cannot be stored in the same container.

* **Code size**: Each instantiation generates its own code. On the target,
    only the variant actually used should be linked into the firmware.


## Build and Test

```
$ cmake -S . -B build       # Generate the build system in ./build
$ cmake --build build       # Build the test executable
$ ./build/test/test         # Run the unit tests
```

All variants are tested in the same run:
```
[==========] Running 7 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 7 tests from DisplayTestGroup
[ RUN      ] DisplayTestGroup.LcdClearTest
Initializing I2C: address=0x27, SDA=21, SCL=22
[       OK ] DisplayTestGroup.LcdClearTest (0 ms)
[ RUN      ] DisplayTestGroup.LcdPrintCharTest
Initializing I2C: address=0x27, SDA=21, SCL=22
[       OK ] DisplayTestGroup.LcdPrintCharTest (0 ms)
[ RUN      ] DisplayTestGroup.OledPrintCharTest
Initializing SPI: MOSI=10, MISO=9, SCK=11, CS=8
[       OK ] DisplayTestGroup.OledPrintCharTest (0 ms)
[ RUN      ] DisplayTestGroup.DisplaySizeTest
[       OK ] DisplayTestGroup.DisplaySizeTest (0 ms)
[ RUN      ] DisplayTestGroup.MockBusInitTest
[       OK ] DisplayTestGroup.MockBusInitTest (0 ms)
[ RUN      ] DisplayTestGroup.MockBusWriteTest
[       OK ] DisplayTestGroup.MockBusWriteTest (0 ms)
[ RUN      ] DisplayTestGroup.OverflowTest
[       OK ] DisplayTestGroup.OverflowTest (0 ms)
[----------] 7 tests from DisplayTestGroup (0 ms total)

[----------] Global test environment tear-down
[==========] 7 tests from 1 test suite ran. (0 ms total)
[  PASSED  ] 7 tests.
```


_Egon Teiniker, 2025-2026, GPL v3.0_
