#include <cstring>
#include <cstdint>
#include <cstdio>

#include <display.h>

// LCD Display Implementation

uint16_t _address; 
uint8_t _sda; 
uint8_t _scl;
uint8_t _data;

void i2c_init(uint16_t address, uint8_t sda, uint8_t scl);
void i2c_write(uint8_t data);


Display::Display(size_t cols, size_t rows) 
	: _cols{cols}, _rows{rows}
{	
	printf("LCD Display Created: %zu cols x %zu rows\n", cols, rows);
	
	_buffer = new char[_cols * _rows]; // malloc(cols_*rows_*sizeof(char))
	_index = 0;

	printf("Initializing I2C for LCD...\n");
	i2c_init(0x27, 21, 22); // Example I2C address and pins
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
	i2c_write((uint8_t)c);
}

char* Display::buffer(void) const
{
	return _buffer;
}

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
