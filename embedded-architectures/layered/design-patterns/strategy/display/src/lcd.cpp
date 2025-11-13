#include <cstring>

#include <lcd.h>

LCD::LCD(size_t cols, size_t rows) 
	: _cols{cols}, _rows{rows}
{	
	_buffer = new char[_cols * _rows]; // malloc(cols_*rows_*sizeof(char))
	_index = 0;
}

LCD::~LCD()
{
	delete[] _buffer; // free();
} 
				
void LCD::clear(void)
{
	_index = 0;
	for(unsigned int i = 0; i < _rows * _cols; i++)
		_buffer[i] = '\0';		
}

void LCD::print(char c)
{
	_buffer[_index++] = c;
}

void LCD::print(const char* c_ptr)
{
	strcpy(_buffer + _index, c_ptr);
	_index += sizeof(c_ptr);
}

char* LCD::buffer(void) const
{
	return _buffer;
}

void LCD::init_i2c(uint16_t address, uint8_t sda, uint8_t scl)
{
	_address = address;
	_sda = sda;
	_scl = scl;

	// Initialize I2C communication
}

void LCD::write_i2c(uint8_t data)
{
	_data = data;

	// Write data to I2C
}
