#ifndef _GPIO_INTERFACE_H_
#define _GPIO_INTERFACE_H_

#include <string>
#include <cstdint>

enum class PinMode : uint8_t
{
	INPUT = 0,
	OUTPUT = 1
};

class Gpio 
{
	public:                
		virtual ~Gpio(void) = default;

		virtual void setPinMode(uint8_t pin, PinMode mode) = 0;
		virtual void writePin(uint8_t pin, bool value) = 0;
		virtual bool readPin(uint8_t pin) = 0;		
		virtual void togglePin(uint8_t pin) = 0;
 };

#endif /* _GPIO_INTERFACE_H_ */
