#include <stdexcept>
#include <regex>

#include "can_proxy.h"

using namespace std;

void CanProxy::sendMessage(const CanMessage& message) 
{
    // Validate CAN ID (11-bit standard)
    if (message.id() > 0x7FF) 
    {
        throw invalid_argument("Invalid CAN ID");
    }

    // Validate data length
    if (message.length() > 8) 
    {
        throw invalid_argument("Invalid CAN data length");
    }

    // Forward the validated message to the actual service
    service->sendMessage(message);
}

CanMessage CanProxy::receiveMessage(void) 
{
    // Receive message from the actual service
    CanMessage message = service->receiveMessage();
    return message;
}
