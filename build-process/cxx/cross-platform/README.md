# C/C++ Cross Platform Build 

Embedded systems (routers, IoT devices, phones) often use different CPU architectures 
(ARM, MIPS, RISC-V, PowerPC, etc.) than your PC (usually x86_64).
These devices may lack the resources (RAM, storage, OS) to compile software themselves.
So we compile on a powerful host PC and run the program on the smaller target device.

## Cross Compiler Toolchain

A toolchain is a collection of programming tools used to develop software for 
a target system. A cross compiler toolchain specifically allows you to build 
code on one system (the host) that will run on a different system (the target).

Components of a Toolchain:

* Compiler (e.g., GCC)
    Converts source code into machine code for the target architecture.

* Assembler
    Translates assembly language into target machine instructions.

* Linker
    Combines object files into executables or libraries for the target.

* C library (e.g., glibc, musl, uClibc)
    Provides standard runtime functions (I/O, memory management, etc.) for the target.

* Binutils
    Includes tools like as (assembler), ld (linker), objdump, readelf, etc.


## Crosstool-NG Framework

crosstool-NG is an open-source framework for building custom cross-compilation toolchains. 
It automates the otherwise tedious process of downloading, configuring, and building all 
components of a toolchain.

Crosstool-NG provides:
* Menu-based configuration (like Linux kernel menuconfig) to select:
    * Target architecture (ARM, MIPS, RISC-V, etc.)
    * C library (glibc, uClibc, musl)
    * GCC version, binutils version, kernel headers, etc.

* Automatic fetching and building of sources (GCC, binutils, libraries).
* Reproducibility: You can export and share configuration files.
* Flexibility: Useful for embedded Linux, bare-metal development, or cross compiling applications.

_Example:_ Build a cross compiler toolchain for a Rspberry Pi 5

### 1. Build Crosstool-NG 

```bash
$ cd Downloads
$ sudo apt update 
$ sudo apt install -y gcc g++ gperf bison flex texinfo help2man make libncurses5-dev \
    python3-dev autoconf automake libtool libtool-bin gawk wget bzip2 xz-utils unzip \
    patch libstdc++6 rsync git meson ninja-build

$ git clone https://github.com/crosstool-ng/crosstool-ng.git
$ cd crosstool-ng

$ ./bootstrap
$ ./configure --prefix=/opt/ct-ng
$ make
$ sudo make install

code .bashrc
export CT_NG_HOME=/op/ct-ng 
To the PATH environment variable add: /opt/ct-ng/bin 

$ ct-ng --version
GNU Make 4.4.1
Built for x86_64-pc-linux-gnu
Copyright (C) 1988-2023 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.
```

### 2. Build a Raspberry Pi 5 Toolchain

```bash
$ mkdir -p Downloads/raspi5-toolchain
$ cd Downloads/raspi5-toolchain
$ code .config                          // see: config-examples 
$ ct-ng build
$ sudo mv x-tools/ /opt/

export X_TOOLS=/opt/x-tools/aarch64-rpi5-linux-gnu
To the PATH environment variable add: $X_TOOLS/bin
```

We can create a new `.config` file from scratch using the interactive menu-driven 
configurator tool provided by Crosstool-NG.

```bash
$ ct-ng menuconfig
```


Here is an overview about the generated tool chain:

```bash
/opt/x-tools/aarch64-rpi5-linux-gnu
│
├── bin/                <-- Cross-compiler frontends you run
│   ├── aarch64-rpi5-linux-gnu-gcc
│   ├── aarch64-rpi5-linux-gnu-ld
│   ├── aarch64-rpi5-linux-gnu-as
│   └── ...
│
├── lib/                <-- Host-side support libs for GCC/binutils
│   └── libgcc.a, compiler plugins, etc.
│
├── libexec/            <-- Internal GCC programs (cc1, collect2...)
│
├── include/            <-- Host headers needed by GCC itself
│
├── share/              <-- Architecture-independent data (man pages, specs)
│
└── aarch64-rpi5-linux-gnu/   <-- Target sysroot (mini rootfs for Pi 5)
    ├── include/        <-- Target headers (kernel, libc, etc.)
    ├── lib/            <-- Target libraries (glibc/musl, crt1.o, etc.)
    └── ...             <-- Mirrors what /usr/ on the Pi would look like
```

* **aarch64-rpi5-linux-gnu/** This folder usually mirrors the target **root filesystem** structure.
    When we cross compile, GCC and binutils use this directory as the sysroot, so the target 
    headers and libraries are kept separate from host ones.
    We can think of this as a mini version of `/usr/` on the Raspberry Pi, but built on your host.

* **bin/** Contains the actual cross compiler binaries we will call, for example:
    - aarch64-rpi5-linux-gnu-gcc
    - aarch64-rpi5-linux-gnu-ld
    - aarch64-rpi5-linux-gnu-as
    - aarch64-rpi5-linux-gnu-strip

    This is the directory we add to our $PATH environment variable.

* **include/** Host-side include files that the compiler itself needs.
    Mostly headers used by GCC internals and the toolchain runtime, not the target’s headers 
    (those live inside `lib/gcc/aarch64-rpi5-linux-gnu/12.5.0/include`).

* **lib/** Host-side support libraries for the toolchain itself.
    - GCC runtime libraries (libgcc.a, etc.)
    - Compiler plugins
    - Build-time helper libraries

* **libexec/** Internal helper programs used by GCC and binutils.
    These aren’t called directly, but the compiler uses them under the hood.

* **share/** Architecture-independent data files:
    - man pages
    - Documentation
    - Config files, build metadata
    - Some GCC runtime data (like specs files)

Notice the separation of **Host vs Target**:

* **Host-side** stuff (`bin/, lib/, libexec/, include/, share/`): Needed to make the compiler 
    itself work on your PC.

* **Target-side** stuff (`aarch64-rpi5-linux-gnu/`): Mimics the Pi’s /usr/ and contains the 
    target headers & libraries used to build ARM64 binaries.

The **sysroot** is the logical root directory for headers and libraries where gcc 
looks for headers, and ld looks for libraries.

The current sysroot can be printed using the `-print-sysroot` option 
and overridden at runtime using gcc's `--sysroot` option.

```bash
$ aarch64-rpi5-linux-gnu-gcc -print-sysroot
/opt/x-tools/aarch64-rpi5-linux-gnu/aarch64-rpi5-linux-gnu/sysroot
```



### 3. Compile a First C Program

Once the cross toolchain is ready, we can compile C code for your target device.

* run `bin/aarch64-rpi5-linux-gnu-gcc`

* Compiler uses host-side parts:
    - `bin/, lib/, libexec/, include/, share/` to drive compilation.

* Compiler pulls in target sysroot:
    - `aarch64-rpi5-linux-gnu/include` and `aarch64-rpi5-linux-gnu/lib` 
    headers & libs to link against as if building on the Pi itself.

* Output binary: runs on Raspberry Pi 5 (AArch64), not on our PC.

```c
// hello.c
#include <stdio.h>

int main() {
    printf("Hello, Raspberry Pi 5!\n");
    return 0;
}
```

```bash
	$ aarch64-rpi5-linux-gnu-gcc hello.c -o hello
	$ file hello
	hello: ELF 64-bit LSB executable, 
		ARM aarch64, 
		version 1 (SYSV), 
		dynamically linked, 
		interpreter /lib/ld-linux-aarch64.so.1, 
		for GNU/Linux 6.16.0, with debug_info, 
		not stripped
```

Finally, we can transfer `hello` to our Raspberry Pi 5 and run it.


## References 

* [Homepage: crosstool-NG](https://crosstool-ng.github.io/)

* [GitHub: Crosstool-NG](https://github.com/crosstool-ng/crosstool-ng)

* [YouTube (The Linux Foundation): Anatomy of Cross-Compilation Toolchains](https://youtu.be/Pbt330zuNPc?si=GfYWpy40Zb2kV5U6)

* [Documentation for binutils](https://sourceware.org/binutils/docs/)
* [GCC online documentation](https://gcc.gnu.org/onlinedocs/)


*Egon Teiniker, 2025, GPL v3.0*