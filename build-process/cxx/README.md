# C/C++ Build Process for Embedded Systems

## Build Process and Binaries

The build process for C/C++ projects involves several steps to transform 
source code into an executable binary. This typically includes preprocessing, 
compiling, assembling, and linking.

* [From C/C++ Source Code to the Executable](build-steps/README.md)
* [C/C++ Compiler](compiler/README.md)
* [Binutils](binutils/README.md)

* Reverse Enineering
    * [Static Analysis](reverse-engineering/analysis-static/)
    * [Dynamic Analysis](reverse-engineering/analysis-dynamic/)
    * [Decompiler](reverse-engineering/analysis-static/Ghidra/README.md)


## Build Cross Platform Binaries  

In embedded systems, the build process is more complex due to the need 
to consider the target hardware platform, cross-compilation, and resource 
constraints.

* [C/C++ Cross Platform Build using crosstool-NG](cross-platform/README.md)


## Build Linux Distributions  

* [Build Linux Distributions using Yocto](yocto/README.md)
* [Firmware Analysis](reverse-engineering/firmware/)


## References

* [YouTube (The Linux Foundation):Building Bare Metal Toolchains, Crosstool-ng and Yocto Project](https://youtu.be/b0yXASkIIv8?si=tPmf_oFJgk7tPOIk)

*Egon Teiniker, 2025, GPL v3.0*