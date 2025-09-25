# Binutils 

**GNU Binutils** (short for *binary utilities*) is a collection of low-level 
tools for working with object files, libraries, and executables.

They form a core part of any toolchain — whether native (x86\_64 on our PC) 
or cross (AArch64 for Raspberry Pi).

When you write code in C/C++:

1. **Compiler (GCC/Clang)** turns source into assembly or object code (`.o`).
2. **Assembler (`as`)** converts assembly into machine instructions.
3. **Linker (`ld`)** combines object files + libraries into the final executable (`a.out`, ELF binary).
4. **Binutils tools** (like `objdump`, `nm`) let us inspect/debug those binaries.

The **Binutils = essential low-level binary tools** (assembler, linker, object inspectors)
sit between the compiler (GCC/Clang) and the final executable.
Without binutils, GCC couldn’t produce runnable programs.


## Main Components of Binutils

Here are the most important tools we will encounter:

### **as** (the assembler)

* Converts assembly code into machine code object files (`.o`).

    _Example:_

    ```bash
    $ as hello.s -o hello.o
    ```

### **ld** (the linker)

* Links multiple object files + libraries into an executable or shared library.
* Resolves symbols (functions, variables) across files.

    _Example:_

    ```bash
    $ ld hello.o -o hello
    ```

### **ar** (archiver)

* Creates static libraries (`.a` files, like `libm.a`).

    _Example:_

    ```bash
    $ ar rcs libfoo.a foo.o bar.o
    ```

### **nm**

* Lists symbols in an object file/library (functions, variables).
    
    _Example:_

    ```bash
    $ nm libfoo.a
    ```

### **objcopy**

* Copies/strips/filters object files.
* Often used in embedded dev to convert ELF into raw binary (`.bin`).

    _Example:_

    ```bash
    $ $objcopy -O binary hello.elf hello.bin
    ```

### **objdump**

* Disassembler + binary inspector.
* Lets you see the assembly from a compiled program.
    
    _Example:_

    ```bash
    $ objdump -d hello.elf
    ```

### **readelf**

* Displays ELF file info (headers, sections, symbols).
* Similar to `objdump`, but only for ELF format.

    _Example:_

    ```bash
    $ readelf -h hello.elf
    ```

### **strip**

* Removes debug info/symbols from binaries → smaller size.

    _Example:_

    ```bash
    $ strip hello
    ```

## References 

* [GNU Binutils Documentation](https://sourceware.org/binutils/docs/binutils.html)

* [YouTube (Jacob Sorber): How to Inspect Compiled Binaries (binutils, objdump)](https://youtu.be/bWMIpHVRFUo?si=a39rPSsaZV-F2-yc)
* [YouTube (LinuxConf): Binutils for fun and profit: understanding Linux binaries](https://youtu.be/htepUJY8nH8?si=E9IzhniADTLLq1aD)

*Egon Teiniker, 2025, GPL v3.0*