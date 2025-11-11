#ifndef _EEPROM_H_
#define _EEPROM_H_

#include <string>
#include <cstdint>

class EEPROM 
{
	private:
		std::string _type;
		size_t _size; 
		uint8_t* _memory;
 
	public:
		EEPROM(void);
		~EEPROM(); 

		std::string type(void) const;
		std::string vendor(void) const;
		
		uint8_t read(const uint32_t address) const;
		void write(const uint32_t address, const uint8_t value);		
 };

#endif /*_EEPROM_H_ */
