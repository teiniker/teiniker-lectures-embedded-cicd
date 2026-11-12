# Example: Cross-Compiler Using CMake and GTest - UserTable


Given a simple UserTable --[*]-> User implementation:

```bash
├── CMakeLists.txt
├── README.md
├── table.cpp
├── table.h
├── test.cpp
├── user.cpp
└── user.h
```

## Build and Test on the Host Machine

We can simply build and test this example on the host machine using cmake:

```bash
$ cmake -S . -B build  
$ cd build/
$ make

$ ./test
```

## Cross-Platform Build

To build the test executable for a Raspberry Pi (ARM64), configure CMake to 
use the Raspberry Pi cross‑compiler and build in an isolated build directory. 
Ensure the cross‑compiler (aarch64-rpi5-linux-gnu-c++) and target libraries 
(GTest) are available on your host or provided via a toolchain/sysroot.

```bash
$ cmake -S . -B build -DCMAKE_CXX_COMPILER=aarch64-rpi5-linux-gnu-c++ 
$ cd build/
$ make
```

Finally, we copy the test executable to the target device.
```bash
$ scp build/test student@xxx.xxx.xxx.xxx:/home/student/Downloads
```


*Egon Teiniker, 2025-2026, GPL v3.0*
