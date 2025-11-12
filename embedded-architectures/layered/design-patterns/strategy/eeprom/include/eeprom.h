#ifndef _EEPROM_H_
#define _EEPROM_H_

#include <string>
#include <cstdint>

class EEPROM 
{
	protected:
		std::string _type;
		size_t _size; 
		uint8_t* _memory;
 
	public:
		virtual~EEPROM() = default; 

		virtual std::string type(void) const = 0;
		virtual std::string vendor(void) const = 0;

		virtual uint8_t read(const uint32_t address) const = 0;
		virtual void write(const uint32_t address, const uint8_t value) = 0;		
 };

#endif /*_EEPROM_H_ */
