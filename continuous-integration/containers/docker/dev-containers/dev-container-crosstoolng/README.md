# Dev Container for C++ Cross Compilation (Raspberry Pi 5)

In the `dev-container-cxx` example, the container provides a compiler which
builds programs for the **same platform** (x86_64). In this example, the
container provides a **Raspberry Pi 5 cross compiler toolchain**.

So the whole toolchain setup (Crosstool-NG, `raspi5.config`, `PATH`,
Google Test for ARM) is described in a Dockerfile, and every developer
gets exactly the same cross compiler, without installing anything on
the host except Docker and VS Code.


## Project Structure

```
dev-container-crosstoolng/
├── .devcontainer/
│   ├── devcontainer.json
│   ├── Dockerfile
│   ├── raspi5.config           <-- Crosstool-NG configuration
│   └── toolchain-rpi5.cmake    <-- CMake toolchain file
├── CMakeLists.txt
└── test.cpp
```

`raspi5.config` is a copy of
`build-process/cxx/cross-platform/config-examples/raspi5.config`. We need a
copy because Docker can only access files inside the build context.


## Comparison with dev-container-cxx

|                    | `dev-container-cxx`     | `dev-container-crosstoolng`  |
|--------------------|-------------------------|------------------------------|
| Base image         | `alpine:latest`         | `debian:trixie-slim`         |
| Compiler           | `g++` (Alpine package)  | `aarch64-rpi5-linux-gnu-g++` |
| Compiler from      | package manager         | built with Crosstool-NG      |
| Host / target      | x86_64 / x86_64         | x86_64 / aarch64             |
| Dockerfile         | single stage            | multi-stage                  |
| First image build  | < 1 minute              | about 1 hour                 |
| Image size         | small                   | 1.5 GB (toolchain)           |
| Run the program    | `./build/hello`         | QEMU or Raspberry Pi         |
| `postCreateCommand`| `cmake -S . -B build`   | with a toolchain file        |
| `devcontainer.json`| same structure          | same structure               |

The structure of the dev container (`.devcontainer/`, `devcontainer.json`,
no `COPY` of the source code) is the same. The difference is the
**content of the image**: there is no package for our Raspberry Pi 5
toolchain, so the Dockerfile has to build it.

Note that we don't use Alpine here: Crosstool-NG builds a toolchain which
runs on the build host. A toolchain built on Debian (glibc) doesn't run
on Alpine (musl), so both stages use Debian.


## Dockerfile

The Dockerfile is a **multi-stage build**:

* **Stage 1 (`toolchain`)** contains everything we need to **build** the
    toolchain: compilers, autotools, the Crosstool-NG sources and the
    downloaded GCC, glibc and Linux sources. This stage is large.

* **Stage 2** is the dev container. It only copies the **result** of
    stage 1, the directory `/opt/x-tools`, into a small Debian image.

```dockerfile
FROM debian:trixie AS toolchain

# Packages needed to build Crosstool-NG and the toolchain
RUN apt-get update && apt-get install -y --no-install-recommends \
        gcc g++ gperf bison flex texinfo help2man make libncurses-dev \
        python3-dev autoconf automake libtool libtool-bin gawk wget \
        bzip2 xz-utils unzip patch rsync git meson ninja-build file \
        ca-certificates cmake \
    && rm -rf /var/lib/apt/lists/*

# Build and install Crosstool-NG
RUN git clone -q --depth 1 --branch crosstool-ng-1.28.0 \
        https://github.com/crosstool-ng/crosstool-ng.git /tmp/crosstool-ng \
    && cd /tmp/crosstool-ng \
    && ./bootstrap \
    && ./configure --prefix=/opt/ct-ng \
    && make \
    && make install \
    && rm -rf /tmp/crosstool-ng
ENV PATH=/opt/ct-ng/bin:$PATH
```

These are the same steps as in the cross platform chapter, but now they
are **documented and repeatable**. We use a fixed Crosstool-NG version
(`1.28.0`), so the image doesn't change when a new version is released.

```dockerfile
# Crosstool-NG refuses to run as root, so we use an ordinary user
RUN useradd -m builder && mkdir /opt/x-tools && chown builder /opt/x-tools
USER builder
WORKDIR /home/builder/raspi5-toolchain

# Build the Raspberry Pi 5 toolchain into /opt/x-tools (takes a while)
COPY .devcontainer/raspi5.config .config
RUN CT_PREFIX=/opt/x-tools ct-ng build \
    && rm -rf .build ~/src
```

* Commands in a Dockerfile run as `root` by default. `ct-ng build`
    stops with an error when it is started by `root`, so we create the
    user `builder`.

* `raspi5.config` sets the installation directory to
    `${CT_PREFIX:-${HOME}/x-tools}/...`. With `CT_PREFIX=/opt/x-tools`, the
    toolchain is installed directly into
    `/opt/x-tools/aarch64-rpi5-linux-gnu`, the same directory as in the
    Virtual Lab. No `mv` is necessary.

* `.build` (the build directory) and `~/src` (the downloaded sources)
    are several GB. We delete them in the same `RUN` instruction.

```dockerfile
ENV PATH=/opt/x-tools/aarch64-rpi5-linux-gnu/bin:$PATH

# Cross compile Google Test and install it into the sysroot of the target
COPY .devcontainer/toolchain-rpi5.cmake /opt/x-tools/toolchain-rpi5.cmake
RUN git clone -q --depth 1 --branch v1.17.0 \
        https://github.com/google/googletest.git /tmp/googletest \
    && cmake -S /tmp/googletest -B /tmp/googletest/build \
        -DCMAKE_TOOLCHAIN_FILE=/opt/x-tools/toolchain-rpi5.cmake \
        -DCMAKE_INSTALL_PREFIX=/usr \
    && cmake --build /tmp/googletest/build -j "$(nproc)" \
    && DESTDIR=/opt/x-tools/aarch64-rpi5-linux-gnu/aarch64-rpi5-linux-gnu/sysroot \
        cmake --install /tmp/googletest/build \
    && rm -rf /tmp/googletest
```

Our test is linked against Google Test, so we need the Google Test
libraries **for the target** (aarch64). We cross compile them with the
new toolchain and install them into the **sysroot**, where the cross
compiler finds the other target libraries, too:

```
.../sysroot/usr/include/gtest/        <-- headers
.../sysroot/usr/lib/libgtest.a        <-- ARM libraries
.../sysroot/usr/lib/cmake/GTest/      <-- used by find_package(GTest)
```

The second stage is the actual development environment:

```dockerfile
FROM debian:trixie-slim

# Build tools for the host, QEMU to run ARM binaries, and git
RUN apt-get update && apt-get install -y --no-install-recommends \
        make cmake git bash file qemu-user openssh-client ca-certificates \
    && rm -rf /var/lib/apt/lists/*

# Copy the toolchain (including Google Test and the CMake toolchain file)
COPY --from=toolchain /opt/x-tools /opt/x-tools

ENV X_TOOLS=/opt/x-tools/aarch64-rpi5-linux-gnu
ENV PATH=$X_TOOLS/bin:$PATH

# qemu-aarch64 finds the target libraries (ld-linux, libc) in the sysroot
ENV QEMU_LD_PREFIX=$X_TOOLS/aarch64-rpi5-linux-gnu/sysroot
```

* `COPY --from=toolchain` copies files from the first stage. Everything
    else of stage 1 is **not** part of the final image.

* The `ENV` instructions replace the changes to `.bashrc` from the cross
    platform chapter. They are valid for every process in the container,
    not only for interactive shells.

* `qemu-user` provides `qemu-aarch64`, which executes ARM binaries on the
    x86_64 host (user mode emulation). `QEMU_LD_PREFIX` tells it where to
    find the ARM dynamic linker and libraries.

* `openssh-client` provides `scp` and `ssh` to copy the program to the
    Raspberry Pi.


## CMake Toolchain File

In the cross platform chapter, we selected the cross compiler with
`-DCMAKE_CXX_COMPILER=aarch64-rpi5-linux-gnu-c++`. This also works in the
container (see [Build the Test](#build-the-test)), but CMake then still
searches the **host** directories (`/usr`, `/usr/local`) for libraries
and packages, **before** the sysroot. If an x86_64 version of a library
is installed there, CMake uses it, and the link step fails.

A **toolchain file** describes the complete target platform:

```cmake
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
```

* `CMAKE_SYSTEM_NAME`: Setting it tells CMake that we are **cross
    compiling** (`CMAKE_CROSSCOMPILING` is `TRUE`).
* `CMAKE_SYSROOT`: Passed to the compiler as `--sysroot`, and used as
    root for all searches.
* `CMAKE_FIND_ROOT_PATH_MODE_*`: `find_package()`, `find_library()` and
    `find_path()` only search the sysroot, so we never link an x86_64
    library by mistake. Programs like `cmake` itself run on the host.

The same file is used to build Google Test in the Dockerfile and our
project in the container.


## devcontainer.json

```json
{
    "name": "C++ Cross Compiler (Raspberry Pi 5)",
    "build": {
        "dockerfile": "Dockerfile",
        "context": ".."
    },
    "customizations": {
        "vscode": {
            "extensions": [
                "ms-vscode.cpptools",
                "ms-vscode.cmake-tools"
            ]
        }
    },
    "postCreateCommand": "cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=/opt/x-tools/toolchain-rpi5.cmake"
}
```

The file is the same as in `dev-container-cxx`, except for the name and
the `postCreateCommand`, which configures the build with the toolchain
file.


## Open the Project in a Dev Container

Requirements: Docker and VS Code with the **Dev Containers** extension.

1. Open the `dev-container-crosstoolng` directory in VS Code.
2. Use **Reopen in Container** (or the command palette `Ctrl+Shift+P`:
    **Dev Containers: Reopen in Container**).
3. The first start builds the image. Building GCC, glibc and the other
    components of the toolchain takes **about an hour**
    (48 minutes on a VM with 4 CPU cores). Later starts reuse the image.
4. The status bar shows
    **Dev Container: C++ Cross Compiler (Raspberry Pi 5)**.

We can also build the image without VS Code, for example to watch the
progress of Crosstool-NG:

```bash
$ docker build -f .devcontainer/Dockerfile -t dev-crosstoolng .
```


## Build the Test

Open a terminal in VS Code (**Terminal > New Terminal**). This terminal
runs inside the container:

```bash
$ aarch64-rpi5-linux-gnu-c++ --version
aarch64-rpi5-linux-gnu-c++ (crosstool-NG 1.28.0.1_403899e) 12.5.0
```

The `postCreateCommand` has already configured the `build` directory with
the toolchain file. To configure the build ourselves, we delete it first
and select the cross compiler with `CMAKE_CXX_COMPILER`:

```bash
$ rm -rf build
$ cmake -S . -B build -DCMAKE_CXX_COMPILER=aarch64-rpi5-linux-gnu-c++
-- The CXX compiler identification is GNU 12.5.0
...
-- Found GTest: /opt/x-tools/aarch64-rpi5-linux-gnu/aarch64-rpi5-linux-gnu/sysroot/usr/lib64/cmake/GTest/GTestConfig.cmake (found version "1.17.0")
-- Configuring done (1.2s)
-- Generating done (0.0s)
-- Build files have been written to: /workspaces/dev-container-crosstoolng/build
```

* `-DCMAKE_CXX_COMPILER=aarch64-rpi5-linux-gnu-c++`: Use the cross
    compiler instead of the default `c++`. It is found via the `PATH`,
    which we set in the Dockerfile.

* `Found GTest`: CMake also searches the sysroot of the cross compiler
    (`.../sysroot/usr`), where we installed the ARM version of Google
    Test in the Dockerfile.

Then we build the test executable:

```bash
$ cmake --build build
...
/opt/x-tools/aarch64-rpi5-linux-gnu/bin/aarch64-rpi5-linux-gnu-c++   -g -std=gnu++17 -Wall -Wextra -Wpedantic ... -c /workspaces/dev-container-crosstoolng/test.cpp
/opt/x-tools/aarch64-rpi5-linux-gnu/bin/aarch64-rpi5-linux-gnu-c++ -g ... CMakeFiles/test.dir/test.cpp.o -o test  -lgtest -lgtest_main -lpthread
[100%] Built target test
```

The verbose output (`CMAKE_VERBOSE_MAKEFILE` in `CMakeLists.txt`) shows
that the cross compiler is used for compiling and linking.

```bash
$ file build/test

build/test: ELF 64-bit LSB executable, ARM aarch64, version 1 (GNU/Linux), dynamically linked, interpreter /lib/ld-linux-aarch64.so.1, for GNU/Linux 6.16.0, with debug_info, not stripped
```

The result is an **ARM aarch64** executable, which can't be executed in
the (x86_64) container:

```bash
$ ./build/test
bash: ./build/test: cannot execute binary file: Exec format error
```

### Run the Test with QEMU

With QEMU user mode emulation, we can run the ARM executable in the
container. QEMU translates the ARM instructions into x86_64 instructions:

```bash
$ qemu-aarch64 build/test
Running main() from /tmp/googletest/googletest/src/gtest_main.cc
[==========] Running 4 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 4 tests from VectorTest
[ RUN      ] VectorTest.Length
[       OK ] VectorTest.Length (2 ms)
...
[==========] 4 tests from 1 test suite ran. (16 ms total)
[  PASSED  ] 4 tests.
```

QEMU only emulates the CPU, not the hardware of the Raspberry Pi (GPIO,
I2C, SPI). Tests which access the hardware must run on the target.

### Run the Test on the Raspberry Pi

Finally, we copy the test executable to the target device and run it
there:

```bash
$ scp build/test student@<ip address>:/home/student/Downloads
$ ssh student@<ip address> /home/student/Downloads/test
```


## References
* [Crosstool-NG](https://crosstool-ng.github.io/)
* [CMake: Cross Compiling](https://cmake.org/cmake/help/latest/manual/cmake-toolchains.7.html#cross-compiling)
* [Docker: Multi-stage builds](https://docs.docker.com/build/building/multi-stage/)
* [QEMU: User Mode Emulation](https://www.qemu.org/docs/master/user/main.html)
* [Development Containers](https://containers.dev/)

*Egon Teiniker, 2025-2026, GPL v3.0*
