# CMake toolchain file for the Raspberry Pi 5 cross compiler
# (built with Crosstool-NG from raspi5.config)

# Target system
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

# Cross compiler (found via PATH)
set(CMAKE_C_COMPILER   aarch64-rpi5-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER aarch64-rpi5-linux-gnu-g++)

# Target root file system (headers and libraries of the target)
set(CMAKE_SYSROOT
    /opt/x-tools/aarch64-rpi5-linux-gnu/aarch64-rpi5-linux-gnu/sysroot)

# Programs (cmake, make, ...) are taken from the host,
# headers, libraries and CMake packages only from the sysroot.
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
