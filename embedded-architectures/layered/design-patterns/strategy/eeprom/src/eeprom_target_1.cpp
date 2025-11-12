#include <eeprom_target_1.h>

using namespace std;

EEPROMTarget1::EEPROMTarget1(void)
{
	_type = "AT28C64B";
	_size = 1024*8; //64Kbit (8Kbit x 8) Parallel EEPROM
	_memory = new uint8_t[_size];
}

EEPROMTarget1::~EEPROMTarget1()
{
	delete[] _memory;  // free(memory_)
} 

string EEPROMTarget1::type(void) const 
{
	return _type;
}

std::string EEPROMTarget1::vendor(void) const
{
	return "Atmel";
}

uint8_t EEPROMTarget1::read(const uint32_t address) const
{
	// 0 <= address < size
	return _memory[address]; 
}

void EEPROMTarget1::write(const uint32_t address, const uint8_t value)
{
	// 0 <= address < size
	_memory[address] = value;
}
