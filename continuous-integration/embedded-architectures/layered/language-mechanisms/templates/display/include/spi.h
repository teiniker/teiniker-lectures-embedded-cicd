#ifndef SPI_H
#define SPI_H

#include <cstdint>
#include <cstdio>

// SPI bus: pins are compile-time constants
template <uint8_t MOSI, uint8_t MISO, uint8_t SCK, uint8_t CS>
class SPI
{
	public:
		static constexpr uint8_t mosi = MOSI;
		static constexpr uint8_t miso = MISO;
		static constexpr uint8_t sck = SCK;
		static constexpr uint8_t cs = CS;

		void init(void)
		{
			printf("Initializing SPI: MOSI=%u, MISO=%u, SCK=%u, CS=%u\n",
				mosi, miso, sck, cs);

			// Initialize SPI communication
		}

		void write(const uint8_t data)
		{
			(void)data;

			// Write data to SPI
		}
};

#endif /* SPI_H */
