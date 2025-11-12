# Example: Conditional Compilation 

If we want to use the same functionality on different embedded systems, we 
often have to **exchange parts of the code within a function**. 

This can be achieved using **pre-processor directives** for conditional compilation.

* `#ifdef`: **If defined** - Checks whether a macro is defined.

* `#ifndef`: **If not defined** - The opposite of `#ifdef`.

* `#if`: **If expression is true** - Allows numeric or boolean-style expressions.
    Can use operators: `==`, `!=`, `>`, `<`, `>=`, `<=`, `&&`, `||`.

* `#else`: **If previous condition fails** - Works with `#ifdef`, `#ifndef`, or `#if`.

* `#endif`: **Closes the conditional block** - Every `#if`, `#ifdef`, and 
    `#ifndef` must end with a matching `#endif`.


_Example_: Conditional compilation - Compiles code only if `FEATURE_ENABLED` has been defined.
```C++
#ifdef FEATURE_ENABLED
    printf("Feature is ON\n");
#endif
```

This is equivalent to:
```C++
#if defined(FEATURE_ENABLED)

```

_Example_: AVR Arduino Core - [HardwareSerial.h](https://github.com/arduino/ArduinoCore-avr/blob/master/cores/arduino/HardwareSerial.h)

```C++
#if defined(UBRRH) || defined(UBRR0H)
  extern HardwareSerial Serial;
  #define HAVE_HWSERIAL0
#endif
#if defined(UBRR1H)
  extern HardwareSerial Serial1;
  #define HAVE_HWSERIAL1
#endif
#if defined(UBRR2H)
  extern HardwareSerial Serial2;
  #define HAVE_HWSERIAL2
#endif
#if defined(UBRR3H)
  extern HardwareSerial Serial3;
  #define HAVE_HWSERIAL3
#endif
```


## Common Interface (same public API)

Although preprocessor directives can be used in headers, keep the public
API stable and **implement platform-specific variants in implementation
files (`.cpp`)**.


```c++
#ifndef _EEPROM_H_
#define _EEPROM_H_

#include <string>
#include <cstdint>

class EEPROM 
{
	private:
		std::string _type;
		size_t _size; 
		uint8_t* _memory;
 
	public:
		EEPROM(void);
		~EEPROM(); 

		std::string type(void) const;
		std::string vendor(void) const;
		
		uint8_t read(const uint32_t address) const;
		void write(const uint32_t address, const uint8_t value);		
 };

#endif /*_EEPROM_H_ */
```

The advantage of this approach is that it also works in C.


## Implementation Variants

All implementation variants can live in the same source file (inside 
the same function or class implementation) and be selected at 
compile time with **preprocessor conditionals** (`#if`, `#elif`, 
`#else`,  `#endif`). 

This is convenient for **small, localized differences**, the 
`vendor()` example is a typical case:

```c++
std::string EEPROM::vendor(void) const
{
#ifdef TARGET_1
	return "Atmel";
#elif defined(TARGET_2)
	return "Microchip";
#else
	return "Unknown";
#endif
}
```

To **confirm the preprocessor** chose the expected branch, we run the preprocessor 
and inspect the resulting translation unit. 

_Example:_ Pre-processor output

```bash
$ cpp -P -Iinclude -DTARGET_1 src/eeprom.cpp
```

* `-P`: Tells the preprocessor not to generate linemarkers (e.g. `# 2 "src/eeprom.cpp" 2`)
* `-Iinclude`: Add the `include` directory to the include path
* `-DTARGET_1`: Define a preprocessor symbol (macro)
* `src/eeprom.cpp`: Input source file

The output shows which code remained after evaluating `#if`, `#ifdef`, 
`#elif`, `#else`.
This helps detect missing or misspelled macro definitions and unintended 
inclusions. 


## Implementation Selection at Link-Time

We use the following project layout: 

```bash
├── README.md
├── CMakeLists.txt
├── include
│   └── eeprom.h
├── src
│   ├── CMakeLists.txt
│   └── eeprom.cpp
└── test
    ├── CMakeLists.txt
    └── test.cpp
```

* `include`: contains the header files that define the **common API**. 
    These headers are provided for all target platforms and form the 
    interface used by the test client.

* `src`: Implementation of the common API for **all target platforms**.

* `test`: Test cases that use the common API.


We must tell CMake which variant to build into the
test executable.
Add a macro definition in the top-level `CMakeLists.txt` file:

```cmake
# Target selection (using pre-processor definitions)
add_definitions(-DTARGET_1)
#add_definitions(-DTARGET_2)
```

When invoking CMake, we can build the selected variant:

```bash
$ cmake -S . -B build 
```

## Consequences of Conditional Compilation

**Advantages**:
- Keeps related code together, easy to see all variants in one place.
- Simple to implement for trivial platform-specific differences.

**Disadvantages**:
- Adds `#ifdef` clutter and **reduces readability** as differences grow.
- Prevents compiling multiple variants simultaneously for testing.
- Larger platform-specific implementations become **hard to maintain**.

**Recommendation**: Use in-source `#if` guards for **small, simple differences**.
Prefer separate implementation files and link-time polymorphism for larger or 
multiple-platform implementations to keep code clear and testable.


## References

* [Designing a HAL in C++](https://blog.mbedded.ninja/programming/languages/c-plus-plus/designing-a-hal-in-cpp/)

_Egon Teiniker, 2025, GPL v3.0_