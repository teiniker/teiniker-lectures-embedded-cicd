#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <cstddef> 	

class Display 
{
	private:
		size_t _cols;
		size_t _rows; 		
		size_t _index;
		char* _buffer;

	public:
		Display(size_t cols, size_t rows); 
		~Display(void); 	

		// Pure virtual Methods
		void clear(void);
		void print(char c);
		char* buffer(void) const;
};

#endif /*_DISPLAY_H_ */
