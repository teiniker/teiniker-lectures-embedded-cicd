# Add the test executable
add_executable(test test.cpp)

# Link libraries to the test executable
target_link_libraries(test PRIVATE eeprom_target gtest gtest_main pthread)
