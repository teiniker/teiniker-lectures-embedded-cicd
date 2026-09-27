cmake_minimum_required(VERSION 3.25)

project(eeprom-target LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include the 'include' directory for header files
include_directories(${PROJECT_SOURCE_DIR}/include)

# Enable compiler warnings globally, activate debugging, and check for gtest
add_compile_options(-Wall -Wextra -Wpedantic)
set(CMAKE_BUILD_TYPE Debug)
find_package(GTest REQUIRED)

# Define a string cache variable with two valid values
set(USE_IMPL "LCD" CACHE STRING "Choose implementation: LCD or OLED")
set_property(CACHE USE_IMPL PROPERTY STRINGS LCD OLED)

# Add subdirectories

# Conditionally add the appropriate source directory based on the variable
if(USE_IMPL STREQUAL "LCD")
    add_subdirectory(src-lcd)
elseif(USE_IMPL STREQUAL "OLED")
    add_subdirectory(src-oled)
else()
    message(FATAL_ERROR "Invalid value for USE_IMPL: ${USE_IMPL}. Valid options are 'LCD' or 'OLED'.")
endif()

add_subdirectory(test)
