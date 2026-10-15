#ifndef _MOTOR_CONTROLLER_H_
#define _MOTOR_CONTROLLER_H_

#include <cstdint>

#include "can_message.h"

template<typename CanBus>
class MotorController 
{
private:
	CanBus& _can;
	uint8_t _current_speed;

public:              
	explicit MotorController(CanBus& can) : _can(can), _current_speed(0) {}
	~MotorController(void) = default;

	void speed(uint8_t km_per_hour)
	{
		_current_speed = km_per_hour;
	}

	void control(void)
	{
		uint8_t actual_speed = readSpeed();
		if(actual_speed < _current_speed)
		{
			setMotorPower(100); // Full power
		}
		else if(actual_speed > _current_speed)
		{
			setMotorPower(0);   // Cut power
		}
		else
		{
			// Do nothing, maintain current power
		}
	}
	
private:
	uint8_t readSpeed(void)	// in km/h
	{
		uint8_t speed = 0;
		CanMessage message = _can.receiveMessage();
		if(message.id() == 0x33)  // Example CAN ID for speed feedback
		{
			speed = message.data()[0];
		}
		return speed;
	}

	void setMotorPower(uint8_t power)	// in %
	{
		CanMessage message(0x22, 1, &power);
		_can.sendMessage(message);
	}
};

#endif /* _MOTOR_CONTROLLER_H_ */
