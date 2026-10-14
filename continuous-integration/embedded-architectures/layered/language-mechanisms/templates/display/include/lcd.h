#ifndef LCD_H
#define LCD_H

#include "display.h"
#include "i2c.h"

// 16x2 character LCD connected via I2C (address 0x27, SDA=21, SCL=22)
using LCD = Display<I2C<0x27, 21, 22>, 16, 2>;

#endif /* LCD_H */
