# Example: Link-Time Polymorphism

Link-time selection of implementation means that **multiple compiled 
versions of the same API** (same headers, same symbols) exist, and 
the build system decides which implementation’s object files or 
libraries are linked into the final executable.

No code changes, no `#ifdef`, no runtime logic—only a build-time switch.


## Common Interface (same public API)

We define the API using header files.
Ensure header files contain no implementation-specific code.

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

Each implementation directory (`src-target-1` and `src-target-2`) contains the
variant `.cpp` files for that target.

```c++
// src-target-1/eeprom.cpp
std::string EEPROM::vendor(void) const
{
	return "Atmel";
}
```

```c++
// src-target-2/eeprom.cpp
std::string EEPROM::vendor(void) const
{
	return "Microchip";
}
```

These variants can be implemented completely differently, as long as they
conform to the requirements specified in the header files.


## Implementation Selection at Link-Time

We use the following project layout: 

```bash
├── README.md
├── CMakeLists.txt
├── include
│   └── eeprom.h
├── src-target-1
│   ├── CMakeLists.txt
│   └── eeprom.cpp
├── src-target-2
│   ├── CMakeLists.txt
│   └── eeprom.cpp
└── test
    ├── CMakeLists.txt
    └── test.cpp
```

* `include`: contains the header files that define the **common API**. 
    These headers are provided for all target platforms and form the 
    interface used by the test client.

* `src-target-1`: Implementation of the common API for target platform 1.

* `src-target-2`: Implementation of the common API for target platform 2.

* `test`: Test cases that use the common API.


We must tell CMake which implementation to build and link into the
test executable.
Add a selection in the top-level `CMakeLists.txt` file:

```cmake
# Define a string cache variable with two valid values
set(USE_IMPL "target-1" CACHE STRING "Choose implementation: target-1 or target-2")
set_property(CACHE USE_IMPL PROPERTY STRINGS target-1 target-2)

# Add subdirectories

# Conditionally add the appropriate source directory based on the variable
if(USE_IMPL STREQUAL "target-1")
    add_subdirectory(src-target-1)
elseif(USE_IMPL STREQUAL "target-2")
    add_subdirectory(src-target-2)
else()
    message(FATAL_ERROR "Invalid value for USE_IMPL: ${USE_IMPL}. Valid options are 'target-1' or 'target-2'.")
endif()
```

When invoking CMake, we can **choose whether to build for target-1
or target-2**.
If we omit the `-DUSE_IMPL` option, the **default variant** ("target-1") is
built.
If you specify an **invalid variant**, CMake will emit an error message.

```bash
# Build for target 1
$ cmake -S . -B build -DUSE_IMPL=target-1

# Build for target 2
$ cmake -S . -B build -DUSE_IMPL=target-2

# Build for target 1 (default)
$ cmake -S . -B build 

# Invalid variant
$ cmake -S . -B build -DUSE_IMPL=target-X
CMake Error at CMakeLists.txt:28 (message):
  Invalid value for USE_IMPL: target-X.  Valid options are 'target-1' or 'target-2'.
```

## Benefits of Link-Time Polymorphism

* **Zero Runtime Overhead**
    - No virtual function tables
    - No function-pointer indirection
    - The compiler can inline, optimize, and dead-strip code
    - This can outperform runtime polymorphism significantly—important 
    in embedded systems, real-time applications, or tight loops.

* **Small, Predictable Binary**
    - Only one implementation is linked
    - Unused code is not even compiled (depending on setup)
    - No plugins or dynamic loaders
    - Ideal for firmware, constrained systems, or controlled deployments.

* **No `#ifdef` Pollution in Code**
    - The implementation switch happens in the build system, 
        not in source files.

Link-time polymorphism trades runtime flexibility for performance, 
simplicity, and clean separation. It’s ideal where speed and binary 
minimalism matter, and less ideal where users expect runtime 
configurability.

## References

* [Designing a HAL in C++](https://blog.mbedded.ninja/programming/languages/c-plus-plus/designing-a-hal-in-cpp/)

_Egon Teiniker, 2025, GPL v3.0_