cmake_minimum_required(VERSION 3.10)

project(blink LANGUAGES C)

# Match `-Wall`
add_compile_options(-Wall)

# Check for WiringPi
find_library(WIRINGPI_LIB wiringPi REQUIRED)

add_executable(blink blink.c)

# Link against WiringPi (equivalent to -lwiringPi)
target_link_libraries(blink PRIVATE wiringPi)
