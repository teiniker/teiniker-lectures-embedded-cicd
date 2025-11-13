#ifndef _OLED_H_
#define _OLED_H_

#include <cstdlib>
#include <cstdint>

#include "display.h"

class OLED : public Display
{
	private:
		size_t _cols;
		size_t _rows; 		
		size_t _index;
		char* _buffer;
 
	public:
		OLED(size_t cols, size_t rows); 	// Constructor
		~OLED(void); 						// Destructor

		// Accessors
		char* buffer(void) const;

		// Methods
		void clear(void) override;
		void print(char c) override;
		void print(const char * c_ptr) override;

	private:
		u_int8_t _mosi;
		u_int8_t _miso;
		u_int8_t _sck;
		u_int8_t _cs;
		uint8_t _data;

		void init_spi(u_int8_t mosi, u_int8_t miso, u_int8_t sck, u_int8_t cs);
		void write_spi(uint8_t data);
};

#endif /*_OLED_H_ */
