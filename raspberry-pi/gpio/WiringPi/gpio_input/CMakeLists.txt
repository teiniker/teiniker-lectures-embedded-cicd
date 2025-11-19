cmake_minimum_required(VERSION 3.10)

project(gpio_input LANGUAGES C)

# Match `-Wall`
add_compile_options(-Wall)

# Check for WiringPi
find_library(WIRINGPI_LIB wiringPi REQUIRED)

add_executable(gpio_input gpio_input.c)

# Link against WiringPi (equivalent to -lwiringPi)
target_link_libraries(gpio_input PRIVATE wiringPi)
