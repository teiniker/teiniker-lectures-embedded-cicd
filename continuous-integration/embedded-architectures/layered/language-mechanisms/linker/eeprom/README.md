# Example: EEPROM

This example implements the same **64 Kbit (8K x 8) parallel EEPROM**
(`AT28C64B`) as the [pre-processor example](../../pre-processor/eeprom/),
but instead of `#ifdef` blocks inside one source file, each target gets
its **own implementation file**.

```
eeprom/
├── CMakeLists.txt          // Project settings and target selection
├── include/eeprom.h        // Common interface (class declaration)
├── src-target-1/eeprom.cpp // Implementation for target 1 (Atmel)
├── src-target-2/eeprom.cpp // Implementation for target 2 (Microchip)
└── test/test.cpp           // GoogleTest unit tests
```

## Link-Time Polymorphism

The header file `include/eeprom.h` defines the **interface** that is shared
by all targets. The client code (here `test/test.cpp`) only includes this
header and does not know which implementation it will use:

```C++
class EEPROM 
{
	// ...
	public:
		EEPROM(void);
		~EEPROM(); 

		std::string type(void) const;
		std::string vendor(void) const;
		
		uint8_t read(const uint32_t address) const;
		void write(const uint32_t address, const uint8_t value);		
};
```

There are **two implementations** of this class, one per target. They
define exactly the same methods; only the platform-specific parts differ:

_Example_: `src-target-1/eeprom.cpp`
```C++
std::string EEPROM::vendor(void) const
{
	return "Atmel";
}
```

_Example_: `src-target-2/eeprom.cpp`
```C++
std::string EEPROM::vendor(void) const
{
	return "Microchip";
}
```

The compiler translates the client code against the declarations in the
header only. Calls like `eeprom.vendor()` remain **unresolved symbols**
in the object file. It is the **linker** that finally binds these symbols
to the implementation it is given - this is why we call it
**link-time polymorphism**.

Since both implementations define the same symbols, only **one of them**
may be linked into an executable; otherwise the linker reports
`multiple definition` errors.

Compared to conditional compilation:

* **No `#ifdef` clutter**: each implementation file is plain, readable code.
* **No runtime overhead**: unlike virtual functions (runtime polymorphism),
    there is no vtable and no indirect call - the binding happens at build time.
* **Clear separation**: platform-specific code lives in its own directory,
    new targets are added by adding a new directory.
* **One variant per build**: as with conditional compilation, we still need
    a separate build for each target.

## Selecting the Target

Each implementation directory contains its own `CMakeLists.txt` which builds
a static library with the **same name** `eeprom`:
```CMake
# Create a library 
add_library(eeprom STATIC eeprom.cpp) 
```

The test executable simply links against `eeprom`, without knowing which
implementation is behind it (`test/CMakeLists.txt`):
```CMake
target_link_libraries(test PRIVATE eeprom gtest gtest_main pthread)
```

The top-level `CMakeLists.txt` decides which directory is added to the build.
For this, it defines a **cache variable** `USE_IMPL` with the default value
`target-1`. `set_property(... STRINGS ...)` lists the valid values (used by
GUI tools like `ccmake` or `cmake-gui` to offer a drop-down list):
```CMake
# Define a string cache variable with two valid values
set(USE_IMPL "target-1" CACHE STRING "Choose implementation: target-1 or target-2")
set_property(CACHE USE_IMPL PROPERTY STRINGS target-1 target-2)
```

Depending on its value, only one of the implementation directories is added:
```CMake
if(USE_IMPL STREQUAL "target-1")
    add_subdirectory(src-target-1)
elseif(USE_IMPL STREQUAL "target-2")
    add_subdirectory(src-target-2)
else()
    message(FATAL_ERROR "Invalid value for USE_IMPL: ${USE_IMPL}. Valid options are 'target-1' or 'target-2'.")
endif()
```

The value can be set on the command line using `-D`:
```
$ cmake -S . -B build -DUSE_IMPL=target-2
```

An invalid value stops the configuration step:
```
$ cmake -S . -B build -DUSE_IMPL=target-3
CMake Error at CMakeLists.txt:28 (message):
  Invalid value for USE_IMPL: target-3.  Valid options are 'target-1' or
  'target-2'.
```

Note that cache variables are **stored** in `build/CMakeCache.txt`.
Once set, the value is kept for subsequent `cmake` calls on the same build
directory until it is changed explicitly with `-DUSE_IMPL=...`.


## Build and Test

The example uses **CMake** to generate the build system and **GoogleTest**
for the unit tests (`find_package(GTest REQUIRED)`).

Build and test the default implementation (`target-1`):
```
$ cmake -S . -B build       # Generate the build system in ./build
$ cd build
$ make                      # Build the eeprom library and the test executable
$ ./test/test               # Run the unit tests
```

The output shows which implementation has been linked:
```
[==========] Running 2 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 2 tests from EEPROMTest
[ RUN      ] EEPROMTest.PrintVendorTest
EEPROM Vendor: Atmel
[       OK ] EEPROMTest.PrintVendorTest (0 ms)
[ RUN      ] EEPROMTest.ReadWriteTest
[       OK ] EEPROMTest.ReadWriteTest (0 ms)
[----------] 2 tests from EEPROMTest (0 ms total)

[----------] Global test environment tear-down
[==========] 2 tests from 1 test suite ran. (0 ms total)
[  PASSED  ] 2 tests.
```

To switch to the other implementation, reconfigure and rebuild:
```
$ cmake -S . -B build -DUSE_IMPL=target-2
$ cmake --build build
$ ./build/test/test
...
EEPROM Vendor: Microchip
...
```

Because the client code does not change, only the selected library is
compiled and the test executable is **re-linked**.

To build and test both targets in parallel (e.g. in a CI pipeline), we can
use **separate build directories**:
```
$ cmake -S . -B build-target-1 -DUSE_IMPL=target-1
$ cmake -S . -B build-target-2 -DUSE_IMPL=target-2
$ cmake --build build-target-1 && ./build-target-1/test/test
$ cmake --build build-target-2 && ./build-target-2/test/test
```

_Egon Teiniker, 2025-2026, GPL v3.0_
