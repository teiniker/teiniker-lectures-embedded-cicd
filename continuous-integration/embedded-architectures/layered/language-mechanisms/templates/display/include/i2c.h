#ifndef I2C_H
#define I2C_H

#include <cstdint>
#include <cstdio>

// I2C bus: address and pins are compile-time constants
template <uint8_t Address, uint8_t SDA, uint8_t SCL>
class I2C
{
	public:
		static constexpr uint8_t address = Address;
		static constexpr uint8_t sda = SDA;
		static constexpr uint8_t scl = SCL;

		void init(void)
		{
			printf("Initializing I2C: address=0x%02x, SDA=%u, SCL=%u\n",
				address, sda, scl);

			// Initialize I2C communication
		}

		void write(const uint8_t data)
		{
			(void)data;

			// Write data to I2C
		}
};

#endif /* I2C_H */
