cmake_minimum_required(VERSION 3.25)

project(simple-test LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Enable all compiler warnings globally
add_compile_options(-Wall -Wextra -Wpedantic)

# Enable Debugging
set(CMAKE_BUILD_TYPE Debug)

# Enable Verbose Output
#set(CMAKE_VERBOSE_MAKEFILE ON)

# Check for Google Test
find_package(GTest REQUIRED)

add_executable(test network_impl.cpp network_proxy.cpp test_impl.cpp test_proxy.cpp)

target_link_libraries(test PRIVATE  gtest gtest_main pthread)
