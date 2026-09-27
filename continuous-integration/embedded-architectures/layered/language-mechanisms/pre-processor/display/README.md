# Example: Conditional Compilation  - Diaplay


$ cpp -P -Iinclude -DLCD_DISPLAY src/display.cpp



# Target selection (using pre-processor definitions)
add_definitions(-LCD_DISPLAY)
#add_definitions(-OLED_DISPLAY)
