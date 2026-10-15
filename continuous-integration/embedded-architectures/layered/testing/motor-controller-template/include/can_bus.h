#ifndef _CAN_BUS_H_
#define _CAN_BUS_H_

#include "can_message.h"

// Stub of a real CAN bus driver (e.g. based on a HAL or SocketCAN).
class CanBus 
{
public:                
	void sendMessage(const CanMessage& message);
	CanMessage receiveMessage(void);
};

#endif /* _CAN_BUS_H_ */
