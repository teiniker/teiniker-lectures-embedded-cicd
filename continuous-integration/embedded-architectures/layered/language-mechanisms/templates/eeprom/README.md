# Example: EEPROM

This example models a **64 Kbit (8K x 8) parallel EEPROM** (`AT28C64B`).
The chip was originally produced by **Atmel**, which was later acquired by
**Microchip**. Depending on the target we build for, the `vendor()` method
should therefore return a different string, while the rest of the class
stays the same.

In this example we use a **class template**: the target becomes 
a **template parameter**.

```
eeprom/
├── CMakeLists.txt      // Project settings (header-only library)
├── include/eeprom.h    // Target traits and class template (with code)
└── test/test.cpp       // GoogleTest unit tests (both targets)
```

## Target Traits

Each target is described by a small **traits class**: a struct that only
contains compile-time information about the target, but no data and no code.

_Example_: `include/eeprom.h`

```C++
struct Atmel
{
	static constexpr const char* vendor = "Atmel";
};

struct Microchip
{
	static constexpr const char* vendor = "Microchip";
};
```

`static constexpr` members are evaluated by the compiler; since C++17 they
are implicitly `inline`, so no separate definition in a `.cpp` file is needed.


## Class Template

The `EEPROM` class gets two template parameters:

```C++
template <typename Target, std::size_t Size = 1024*8>
class EEPROM 
{
	private:
		std::string _type;
		std::array<uint8_t, Size> _memory; 
	// ...
};
```

* `typename Target`: A **type parameter**, the traits class of the target.

* `std::size_t Size`: A **non-type parameter** with a default value. Because
    the size is known at compile time, we can use `std::array` instead of
    `new[]`/`delete[]`: no heap allocation and no destructor are needed,
    which is important on embedded systems.

The target-specific code is reduced to a single line:

```C++
std::string vendor(void) const
{
	return Target::vendor;
}
```

When the compiler **instantiates** the template for a given type, e.g.
`EEPROM<Atmel>`, it replaces `Target` with `Atmel` and generates the code
of a concrete class. Like conditional compilation, this happens at
**compile time**: there is no runtime overhead (no `if`, no virtual call).


## Header-Only Implementation

A class template is not code yet, it is only a **blueprint** for code.
The compiler can only instantiate it if the **definition** (not only the
declaration) of the template is visible in the translation unit that uses
it. Therefore, the complete implementation is placed in the header file.
The simplest way is to define the member functions **directly inside the
class**:

_Example_: `include/eeprom.h`

```C++
template <typename Target, std::size_t Size = 1024*8>
class EEPROM 
{
	// ...
	public:
		uint8_t read(const uint32_t address) const
		{
			// 0 <= address < Size
			return _memory[address]; 
		}
	// ...
};
```

Alternatively, the member functions can be defined **below the class**
declaration. Then each definition has to repeat the template parameter list
and qualify the name with the class template:

```C++
template <typename Target, std::size_t Size>
uint8_t EEPROM<Target, Size>::read(const uint32_t address) const
{
	// 0 <= address < Size
	return _memory[address]; 
}
```

Both variants generate the same code. Definitions inside the class are
shorter, because the template parameters are already in scope. Definitions
below the class keep the declaration readable as a compact overview of the
interface, which pays off for larger classes.

Note that we use `std::string` instead of `using namespace std;` in the
header, because a `using` directive would leak into every file that
includes it.

Every `.cpp` file that uses e.g. `EEPROM<Microchip>` generates its own copy
of the required member functions (**implicit instantiation**). Member
functions defined inside a class, as well as member functions of class
templates, are implicitly `inline`, so this does not
violate the **One Definition Rule** (ODR): the copies are emitted as
**weak symbols** (`W`) and the linker keeps only one of them.

Only the member functions that are **actually used** are instantiated:

```
$ nm -C build/test/CMakeFiles/test.dir/test.cpp.o | grep 'W EEPROM<'
0000000000000000 W EEPROM<Atmel, 8192ul>::EEPROM()
0000000000000000 W EEPROM<Microchip, 8192ul>::write(unsigned int, unsigned char)
0000000000000000 W EEPROM<Microchip, 8192ul>::EEPROM()
0000000000000000 W EEPROM<Atmel, 8192ul>::vendor[abi:cxx11]() const
0000000000000000 W EEPROM<Microchip, 8192ul>::read(unsigned int) const
0000000000000000 W EEPROM<Microchip, 8192ul>::vendor[abi:cxx11]() const
...
```

The tests never call `type()`, and `read()`/`write()` only for `Microchip`,
so no code is generated for them.

Since there is nothing to compile, there is no `src/` directory. In CMake,
the header-only library is modeled as an **INTERFACE library**, which only
passes its **usage requirements** (here the include path) on to the targets
that link against it:

```CMake
# Header-only library: nothing to compile, only usage requirements
add_library(eeprom INTERFACE)
target_include_directories(eeprom INTERFACE ${PROJECT_SOURCE_DIR}/include)
```

The test executable still uses `target_link_libraries(test PRIVATE eeprom
...)` and gets the include directory automatically.


### Header-Only vs. Explicit Instantiation

Alternatively, the implementation can stay in a `.cpp` file, which then
**explicitly instantiates** all supported variants
(`template class EEPROM<Atmel>;`). The header contains only declarations.

* **Flexibility**: A header-only template works with **any** template
    argument, e.g. a new target or `EEPROM<Atmel, 1024>`. With explicit
    instantiation, a combination that has not been instantiated fails at
    link time; the set of supported targets is **controlled** by the
    library.

* **Simplicity**: A header-only library is just a header file: no library
    build step, no `.cpp` file to keep in sync with the instantiations.

* **Optimization**: The compiler sees the complete code at the call site
    and can inline it.

* **Build time**: Every translation unit that includes the header compiles
    the implementation again, and every change in the implementation causes
    all of them to be recompiled.

* **Encapsulation**: The implementation is visible to all clients and cannot
    be shipped as a precompiled library only.

## Selecting the Target

There are no macros and no compile definitions in `CMakeLists.txt` anymore.
The target is selected in the **source code** by the template argument:

```C++
EEPROM<Microchip> eeprom;
```

In production code, the decision is usually made in **one place**, e.g.
a board configuration header, using a type alias:

```C++
using Eeprom = EEPROM<Microchip>;
```

All other code then uses `Eeprom` and does not depend on a specific target.

## Templates vs. Conditional Compilation

* **All variants in one build**: Both targets are compiled into the same
    executable, so the tests can check both vendors in a single test run.

* **All variants are type-checked**: Code in an inactive `#ifdef` branch is
    never compiled, so errors there stay hidden until somebody builds that
    target. Every instantiated template is checked by the compiler.

* **Typos are detected**: A misspelled macro (`-DTARGT_1`) silently selects
    the `#else` branch, a misspelled type (`EEPROM<Atmell>`) is a compile
    error.

* **Template parameters propagate**: Every function that receives an
    `EEPROM` must either be a template itself or use a type alias.

* **Code size**: Each instantiation generates its own code. On the target,
    only the variant actually used should be linked into the firmware.


## Build and Test

The example uses **CMake** to generate the build system and **GoogleTest**
for the unit tests (`find_package(GTest REQUIRED)`).

```
$ cmake -S . -B build       # Generate the build system in ./build
$ cmake --build build       # Build the test executable
$ ./build/test/test         # Run the unit tests
```

Both targets are tested in the same run:
```
[==========] Running 3 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 3 tests from EEPROMTest
[ RUN      ] EEPROMTest.AtmelVendorTest
[       OK ] EEPROMTest.AtmelVendorTest (0 ms)
[ RUN      ] EEPROMTest.MicrochipVendorTest
[       OK ] EEPROMTest.MicrochipVendorTest (0 ms)
[ RUN      ] EEPROMTest.ReadWriteTest
[       OK ] EEPROMTest.ReadWriteTest (0 ms)
[----------] 3 tests from EEPROMTest (0 ms total)

[----------] Global test environment tear-down
[==========] 3 tests from 1 test suite ran. (0 ms total)
[  PASSED  ] 3 tests.
```


_Egon Teiniker, 2025-2026, GPL v3.0_
