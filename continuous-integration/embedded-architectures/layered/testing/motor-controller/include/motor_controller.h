#ifndef _MOTOR_CONTROLLER_H_
#define _MOTOR_CONTROLLER_H_

#include <string>
#include <cstdint>
#include <memory>

#include "can_interface.h"



class MotorController 
{
private:
	std::shared_ptr<CanInterface> _can;
	uint8_t _current_speed;

public:              
	MotorController(std::shared_ptr<CanInterface> can) : _can(can), _current_speed(0) {}
	~MotorController(void) = default;

	void speed(uint8_t km_per_hour);
	void control(void);
	
private:
	uint8_t readSpeed(void);	// in km/h
	void setMotorPower(uint8_t power);	// in %
};


#endif /* _MOTOR_CONTROLLER_H_ */
