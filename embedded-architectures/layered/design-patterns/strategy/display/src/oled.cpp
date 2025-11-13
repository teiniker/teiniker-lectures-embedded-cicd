#include <cstring>

#include <oled.h>

OLED::OLED(size_t cols, size_t rows) 
	: _cols{cols}, _rows{rows}
{	
	_buffer = new char[_cols * _rows]; // malloc(cols_*rows_*sizeof(char))
	_index = 0;
}

OLED::~OLED()
{
	delete[] _buffer; // free();
} 

void OLED::clear(void)
{
	_index = 0;
	for(unsigned int i = 0; i < _rows * _cols; i++)
		_buffer[i] = '\0';		
}

void OLED::print(char c)
{
	_buffer[_index++] = c;
}

void OLED::print(const char* c_ptr)
{
	strcpy(_buffer + _index, c_ptr);
	_index += sizeof(c_ptr);
}

char* OLED::buffer(void) const
{
	return _buffer;
}

void OLED::init_spi(u_int8_t mosi, u_int8_t miso, u_int8_t sck, u_int8_t cs)
{
	_mosi = mosi;
	_miso = miso;
	_sck = sck;
	_cs = cs;

	// Initialize SPI communication
}

void OLED::write_spi(uint8_t data)
{
	_data = data;

	// Write data to SPI
}
