#ifndef _EEPROM_TARGET_2_H_
#define _EEPROM_TARGET_2_H_

#include <eeprom.h>

class EEPROMTarget2 : public EEPROM
{
	public:
		EEPROMTarget2(void);
		~EEPROMTarget2(void); 

		std::string type(void) const;
		std::string vendor(void) const;

		uint8_t read(const uint32_t address) const;
		void write(const uint32_t address, const uint8_t value);
};

#endif //_EEPROM_TARGET_2_H_
