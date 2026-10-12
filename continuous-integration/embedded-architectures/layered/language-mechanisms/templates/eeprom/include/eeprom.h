#ifndef _EEPROM_H_
#define _EEPROM_H_

#include <string>
#include <array>
#include <cstddef>
#include <cstdint>

// Target configurations (traits classes)
struct Atmel
{
	static constexpr const char* vendor = "Atmel";
};

struct Microchip
{
	static constexpr const char* vendor = "Microchip";
};

template <typename Target, std::size_t Size = 1024*8>
class EEPROM 
{
	private:
		std::string _type;
		std::array<uint8_t, Size> _memory; 
 
	public:
		EEPROM(void) : _memory{}
		{
			_type = "AT28C64B";	//64Kbit (8Kbit x 8) Parallel EEPROM
		}

		std::string type(void) const
		{
			return _type;
		}

		std::string vendor(void) const
		{
			return Target::vendor;
		}
		
		uint8_t read(const uint32_t address) const
		{
			// 0 <= address < Size
			return _memory[address]; 
		}

		void write(const uint32_t address, const uint8_t value)
		{
			// 0 <= address < Size
			_memory[address] = value;
		}
 };

#endif /*_EEPROM_H_ */
