# Install GTest 

We don’t `apt-get gtest` for ARM into a cross toolchain.
Instead, **we cross-compile GoogleTest with our aarch64 compiler** and install 
it into the toolchain’s sysroot so our projects can find headers/libs when 
cross-building.

```bash
$ cd Downloads 
$ git clone https://github.com/google/googletest.git
$ cd googletest/
$ cmake -S . -B build \
    -DCMAKE_SYSTEM_NAME=Linux \
    -DCMAKE_SYSTEM_PROCESSOR=aarch64 \
    -DCMAKE_CXX_COMPILER=aarch64-rpi5-linux-gnu-c++ \
    -DCMAKE_SYSROOT=/opt/x-tools/aarch64-rpi5-linux-gnu/aarch64-rpi5-linux-gnu/sysroot \
    -DCMAKE_INSTALL_PREFIX=/opt/x-tools/aarch64-rpi5-linux-gnu/aarch64-rpi5-linux-gnu/sysroot/usr

$ cd build/
$ make
$ sudo make install
```

The built and **installed header and library files** can be found at:

```bash
/opt/x-tools/aarch64-rpi5-linux-gnu/aarch64-rpi5-linux-gnu/sysroot/usr/include/gtest/
/opt/x-tools/aarch64-rpi5-linux-gnu/aarch64-rpi5-linux-gnu/sysroot/usr/lib
```

Note that libraries installed under `aarch64-rpi5-linux-gnu/aarch64-rpi5-linux-gnu/sysroot/usr/`
are found automatically by the cross-compiler.


*Egon Teiniker, 2025, GPL v3.0*
