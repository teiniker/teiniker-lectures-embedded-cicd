###########################################################################
# Stage 1: Build the cross compiler toolchain with Crosstool-NG
###########################################################################
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

# Crosstool-NG refuses to run as root, so we use an ordinary user
RUN useradd -m builder && mkdir /opt/x-tools && chown builder /opt/x-tools
USER builder
WORKDIR /home/builder/raspi5-toolchain

# Build the Raspberry Pi 5 toolchain into /opt/x-tools (takes a while)
COPY .devcontainer/raspi5.config .config
RUN CT_PREFIX=/opt/x-tools ct-ng build \
    && rm -rf .build ~/src

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


###########################################################################
# Stage 2: Development environment (contains only the result of stage 1)
###########################################################################
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

# Note: No COPY of the source code and no build step here.
# The source code is mounted into the container by VS Code.
