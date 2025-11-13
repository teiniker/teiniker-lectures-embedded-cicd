#include <cstring>
#include <cstdint>
#include <cstdio>

#include <display.h>

// LCD Display Implementation
#ifdef LCD_DISPLAY
uint16_t _address; 
uint8_t _sda; 
uint8_t _scl;
uint8_t _data;

void i2c_init(uint16_t address, uint8_t sda, uint8_t scl);
void i2c_write(uint8_t data);

#elif defined(OLED_DISPLAY)
// OLED Display Implementation

uint8_t _mosi;
uint8_t _miso;
uint8_t _sck;
uint8_t _cs;
uint8_t _data;

void spi_init(uint8_t mosi, uint8_t miso, uint8_t sck, uint8_t cs);
void spi_write(uint8_t data);

#endif

Display::Display(size_t cols, size_t rows) 
	: _cols{cols}, _rows{rows}
{	
#ifdef LCD_DISPLAY
	printf("LCD Display Created: %zu cols x %zu rows\n", cols, rows);
#elif defined(OLED_DISPLAY)
	printf("OLED Display Created: %zu cols x %zu rows\n", cols, rows);
#endif
	
	_buffer = new char[_cols * _rows]; // malloc(cols_*rows_*sizeof(char))
	_index = 0;

#ifdef LCD_DISPLAY
	printf("Initializing I2C for LCD...\n");
	i2c_init(0x27, 21, 22); // Example I2C address and pins
#elif defined(OLED_DISPLAY)
	printf("Initializing SPI for OLED...\n");
	spi_init(10, 9, 11, 8); 
#endif

}

Display::~Display()
{
	delete[] _buffer; // free();
} 

void Display::clear(void)
{
	_index = 0;
	for(unsigned int i = 0; i < _rows * _cols; i++)
		_buffer[i] = '\0';		
}

void Display::print(char c)
{
	_buffer[_index++] = c;

#ifdef LCD_DISPLAY
	i2c_write((uint8_t)c);
#elif defined(OLED_DISPLAY)
	spi_write((uint8_t)c);
#endif
}

char* Display::buffer(void) const
{
	return _buffer;
}

#ifdef LCD_DISPLAY
// I2C Communication Methods

void i2c_init(uint16_t address, uint8_t sda, uint8_t scl)
{
	_address = address;
	_sda = sda;
	_scl = scl;
	
	// Initialize I2C communication
}

void i2c_write(uint8_t data)
{
	_data = data;
	
	// Write data to I2C
}

#elif defined(OLED_DISPLAY)
// SPI Communication Methods

void spi_init(uint8_t mosi, uint8_t miso, uint8_t sck, uint8_t cs)
{
	_mosi = mosi;
	_miso = miso;
	_sck = sck;
	_cs = cs;

	// Initialize SPI communication
}

void spi_write(uint8_t data)
{
	_data = data;

	// Write data to SPI
}

#endif
