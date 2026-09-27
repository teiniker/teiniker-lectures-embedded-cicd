#include <stdexcept>

#include "can_impl.h"

using namespace std;


void CanImpl::sendMessage(const CanMessage& message)
{
    // Simulate sending a CAN message by storing it internally
    _message = message;
}

CanMessage CanImpl::receiveMessage(void)
{
    // Simulate receiving a CAN message by returning the internally stored message
    return _message;
}