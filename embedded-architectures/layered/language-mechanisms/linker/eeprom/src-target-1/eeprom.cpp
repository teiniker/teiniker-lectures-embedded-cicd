#include "eeprom.h"

using namespace std;

EEPROM::EEPROM(void)
{
	_type = "AT28C64B";
	_size = 1024*8; //64Kbit (8Kbit x 8) Parallel EEPROM
	_memory = new uint8_t[_size];
}

EEPROM::~EEPROM()
{
	delete[] _memory;  // free(memory_)
} 

string EEPROM::type(void) const 
{
	return _type;
}

std::string EEPROM::vendor(void) const
{
	return "Atmel";
}

uint8_t EEPROM::read(const uint32_t address) const
{
	// 0 <= address < size
	return _memory[address]; 
}

void EEPROM::write(const uint32_t address, const uint8_t value)
{
	// 0 <= address < size
	_memory[address] = value;
}
