cmake_minimum_required(VERSION 3.25)

project(eeprom-target LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Enable compiler warnings globally, activate debugging, and check for gtest
add_compile_options(-Wall -Wextra -Wpedantic)
set(CMAKE_BUILD_TYPE Debug)
find_package(GTest REQUIRED)

# Header-only library: nothing to compile, only usage requirements
add_library(eeprom INTERFACE)
target_include_directories(eeprom INTERFACE ${PROJECT_SOURCE_DIR}/include)

# Add subdirectories
add_subdirectory(test)

