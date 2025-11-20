#ifndef CAN_IMPL_H
#define CAN_IMPL_H

#include <string>
#include <map>

#include "can.h"

class CanImpl : public CanInterface
{
private:
    CanMessage _message = {0, 0, nullptr};

public:
    void sendMessage(const CanMessage& message) override;
    CanMessage receiveMessage(void) override;
};

#endif // CAN_IMPL_H