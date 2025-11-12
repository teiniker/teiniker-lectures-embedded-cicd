#include <eeprom_target_2.h>

using namespace std;

EEPROMTarget2::EEPROMTarget2(void)
{
	_type = "AT28C64B";
	_size = 1024*8; //64Kbit (8Kbit x 8) Parallel EEPROM
	_memory = new uint8_t[_size];
}

EEPROMTarget2::~EEPROMTarget2(void)
{
	delete[] _memory;  // free(memory_)
} 

string EEPROMTarget2::type(void) const 
{
	return _type;
}

std::string EEPROMTarget2::vendor(void) const
{
	return "Microchip";
}

uint8_t EEPROMTarget2::read(const uint32_t address) const
{
	// 0 <= address < size
	return _memory[address]; 
}

void EEPROMTarget2::write(const uint32_t address, const uint8_t value)
{
	// 0 <= address < size
	_memory[address] = value;
}
