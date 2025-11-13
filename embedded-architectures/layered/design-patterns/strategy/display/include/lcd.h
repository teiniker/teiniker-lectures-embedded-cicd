#ifndef _LCD_H_
#define _LCD_H_

#include <cstdlib>
#include <cstdint>

#include "display.h"

class LCD : public Display
{
	private:
		size_t _cols;
		size_t _rows; 		
		size_t _index;
		char* _buffer;
 
	public:
		LCD(size_t cols, size_t rows); 	// Constructor
		~LCD(void); 					// Destructor

		// Accessors
		char* buffer(void) const;

		// Methods
		void clear(void) override;
		void print(char c) override;

	private:
		uint16_t _address; 
		uint8_t _sda; 
		uint8_t _scl;
		uint8_t _data;

		void i2c_init(uint16_t address, uint8_t sda, uint8_t scl);
		void i2c_write(uint8_t data);
 };

#endif /*_LCD_H_ */
