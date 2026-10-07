# Example: Conditional Compilation - Diaplay

## C Pre-Processor
```bash
$ cpp -P -Iinclude -DLCD_DISPLAY src/display.cpp
```

## Target selection (using pre-processor definitions)
```bash
add_compile_definitions(LCD_DISPLAY)
# add_compile_definitions(OLED_DISPLAY)
```

## Build and Test

```
$ cmake -S . -B build       
$ cd build
$ make                      
$ ./test/test               
```

_Egon Teiniker, 2025-2026, GPL v3.0_
