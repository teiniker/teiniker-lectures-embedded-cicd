#ifndef OLED_H
#define OLED_H

#include "display.h"
#include "spi.h"

// 128x64 OLED with a 6x8 font (21x8 characters) connected via SPI
using OLED = Display<SPI<10, 9, 11, 8>, 21, 8>;

#endif /* OLED_H */
