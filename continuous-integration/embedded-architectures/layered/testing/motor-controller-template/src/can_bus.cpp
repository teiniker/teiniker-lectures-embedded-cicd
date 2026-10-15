#include <iostream>

#include <can_bus.h>

void CanBus::sendMessage(const CanMessage& message)
{
    // Stub: a real driver would write the frame to the CAN controller
    std::cout << "CAN send: id=0x" << std::hex << message.id()
              << " data[0]=" << std::dec << static_cast<int>(message.data()[0])
              << std::endl;
}

CanMessage CanBus::receiveMessage(void)
{
    // Stub: a real driver would read a frame from the CAN controller
    uint8_t data[1] = {30};             // Simulated speed 30 km/h
    return CanMessage(0x33, 1, data);
}
