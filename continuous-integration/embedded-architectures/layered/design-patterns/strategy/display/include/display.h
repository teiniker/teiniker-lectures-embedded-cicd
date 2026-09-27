#ifndef _DISPLAY_H_
#define _DISPLAY_H_

class Display 
{
	public:
		virtual ~Display(void) {}; 	// Destructor

		// Pure virtual Methods
		virtual void clear(void) = 0;
		virtual void print(char c) = 0;
};

#endif /*_DISPLAY_H_ */
