#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include <string>
#include <cstdint>
#include <memory>

#include "gpio_interface.h"

enum class Button : uint8_t
{
	UP = 0,
	DOWN = 1,
	STOP = 2
};

enum class MotorState : uint8_t
{
	STOP = 0,
	FORWARD = 1,
	BACKWARD = 2
};


class Controller 
{
private:
	Gpio* _gpio;

public:              
	Controller(Gpio* gpio) : _gpio(gpio) {}
	~Controller(void) = default;

	void initialize(void);
	void control(void);
	
private:
	Button readButton(void);
	void setMotor(MotorState state);
};


#endif /* _CONTROLLER_H_ */
