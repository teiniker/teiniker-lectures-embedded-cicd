# Example: EEPROM

This example models a **64 Kbit (8K x 8) parallel EEPROM** (`AT28C64B`).
The chip was originally produced by **Atmel**, which was later acquired by
**Microchip**. Depending on the target we build for, the `vendor()` method
should therefore return a different string, while the rest of the class
stays the same.

```
eeprom/
├── CMakeLists.txt      // Project settings and target selection
├── include/eeprom.h    // Class declaration
├── src/eeprom.cpp      // Class implementation (with conditional compilation)
└── test/test.cpp       // GoogleTest unit tests
```

## Conditional Compilation

The **pre-processor** runs before the compiler and decides, based on defined
macros, which lines of source code are passed on to the compiler.
Code in an inactive branch is **removed completely** - it is neither compiled
nor part of the resulting binary.

_Example_: `src/eeprom.cpp`

```C++
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

The following pre-processor directives are used:

* `#ifdef TARGET_1`: The first branch is used if the macro `TARGET_1` is
    **defined** (its value does not matter).

* `#elif defined(TARGET_2)`: **Else-if** checked only if the previous
    condition failed. Since there is no `#elifdef` before C23/C++23, we use
    the `defined()` operator inside an `#elif` expression.

* `#else`: **Fallback** - used if neither `TARGET_1` nor `TARGET_2` is defined.
    Providing a default branch ensures that the code always compiles
    (alternatively, `#error "No target defined"` could stop the build).

* `#endif`: **Closes** the conditional block.

Note that the conditions are checked **in order**: if both macros are defined,
the `TARGET_1` branch wins.

We can use the **pre-processor** directly to see which code the compiler
actually gets (`-P` suppresses line markers, `-D` defines a macro):

```
$ cpp -P -Iinclude -DTARGET_1 src/eeprom.cpp
...
std::string EEPROM::vendor(void) const
{
 return "Atmel";
}
...
```

## Selecting the Target

Macros are usually not defined in the source code but passed to the compiler
via the `-D` option, e.g. `g++ -DTARGET_2 ...`. There are two ways to do this
in our CMake project:

### In CMakeLists.txt

The top-level `CMakeLists.txt` adds the macro definition to all targets
(library and tests) of the project. To switch the target, we comment one line
in and the other one out:
```CMake
# Target selection (using pre-processor definitions)
#add_compile_definitions(TARGET_1)
add_compile_definitions(TARGET_2)
```

`add_compile_definitions()` (CMake 3.12+) takes the macro name **without**
the `-D` prefix; CMake adds the compiler-specific option itself. It replaces
the older `add_definitions(-DTARGET_2)`, which passes arbitrary compiler flags.
To define a macro for a single target only, use
`target_compile_definitions(eeprom PRIVATE TARGET_2)`.

### On the Command Line

Without changing any file, we can pass additional compiler flags when the
build system is generated:
```
$ cmake -S . -B build -DCMAKE_CXX_FLAGS="-DTARGET_1"
```

Note that the outer `-D` defines the **CMake variable** `CMAKE_CXX_FLAGS`,
while the inner `-DTARGET_1` is the **compiler option** that defines the macro.

The definitions from the command line are **added** to those in
`CMakeLists.txt`. In the case above, both `TARGET_1` and `TARGET_2` are
defined, and because `#ifdef TARGET_1` is checked first, the vendor is `"Atmel"`.
To select the target only from the command line, remove the
`add_compile_definitions()` line from `CMakeLists.txt`.

Changing `CMAKE_CXX_FLAGS` requires a **rebuild** of the affected files;
when in doubt, delete the `build` directory and start over.


## Build and Test

The example uses **CMake** to generate the build system and **GoogleTest**
for the unit tests (`find_package(GTest REQUIRED)`).

```
$ cmake -S . -B build       # Generate the build system in ./build
$ cd build
$ make                      # Build the eeprom library and the test executable
$ ./test/test               # Run the unit tests
```

The output shows which vendor string has been compiled into the binary
(here with `TARGET_2` defined in `CMakeLists.txt`):
```
[==========] Running 2 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 2 tests from EEPROMTest
[ RUN      ] EEPROMTest.PrintVendorTest
EEPROM Vendor: Microchip
[       OK ] EEPROMTest.PrintVendorTest (0 ms)
[ RUN      ] EEPROMTest.ReadWriteTest
[       OK ] EEPROMTest.ReadWriteTest (0 ms)
[----------] 2 tests from EEPROMTest (0 ms total)

[----------] Global test environment tear-down
[==========] 2 tests from 1 test suite ran. (0 ms total)
[  PASSED  ] 2 tests.
```

To build the other variant, change the target selection (see above) and
build again:
```
$ rm -rf build
$ cmake -S . -B build -DCMAKE_CXX_FLAGS="-DTARGET_1"
$ cmake --build build
$ ./build/test/test
...
EEPROM Vendor: Atmel
...
```

Note that **only one variant can be built at a time** - to test both vendors,
we need two separate builds. This is one of the main drawbacks of conditional
compilation.


_Egon Teiniker, 2025-2026, GPL v3.0_
