cmake_minimum_required(VERSION 3.25)

project(client-server LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_C_EXTENSIONS OFF)

# Include the 'include' directory for header files
include_directories(${PROJECT_SOURCE_DIR}/include)

# Enable compiler warnings globally and activate debugging.
# Strict C17 hides POSIX functions (getaddrinfo, ...), so request them.
add_compile_options(-Wall -Wextra -Wpedantic)
add_compile_definitions(_POSIX_C_SOURCE=200809L)
set(CMAKE_BUILD_TYPE Debug)

# Add subdirectories
add_subdirectory(src)
