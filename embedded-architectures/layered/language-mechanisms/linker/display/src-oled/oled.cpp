#include <cstring>
#include <cstdint>
#include <cstdio>

#include <display.h>

// OLED Display Implementation

uint8_t _mosi;
uint8_t _miso;
uint8_t _sck;
uint8_t _cs;
uint8_t _data;

void spi_init(uint8_t mosi, uint8_t miso, uint8_t sck, uint8_t cs);
void spi_write(uint8_t data);


Display::Display(size_t cols, size_t rows) 
	: _cols{cols}, _rows{rows}
{	
	printf("OLED Display Created: %zu cols x %zu rows\n", cols, rows);

	_buffer = new char[_cols * _rows]; // malloc(cols_*rows_*sizeof(char))
	_index = 0;
	
	printf("Initializing SPI for OLED...\n");
	spi_init(10, 9, 11, 8); 
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
	spi_write((uint8_t)c);
}

char* Display::buffer(void) const
{
	return _buffer;
}

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
