# Create a library for the CAN bus driver (stub)
add_library(can_bus STATIC can_bus.cpp)

# Production executable: MotorController<CanBus>
add_executable(main main.cpp)
target_link_libraries(main PRIVATE can_bus)
