cmake_minimum_required(VERSION 3.25)

project(mqtt-cxx LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_compile_options(-Wall -Wextra -Wpedantic)

# Eclipse Paho MQTT C++ library (Debian: libpaho-mqttpp-dev)
find_package(PahoMqttCpp REQUIRED)

add_executable(publisher publisher.cpp)
target_link_libraries(publisher PRIVATE PahoMqttCpp::paho-mqttpp3)

add_executable(subscriber subscriber.cpp)
target_link_libraries(subscriber PRIVATE PahoMqttCpp::paho-mqttpp3)
