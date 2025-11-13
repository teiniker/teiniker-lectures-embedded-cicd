#include <gtest/gtest.h>

#include <lcd.h>
#include <oled.h>

TEST(LCDTestGroup, ClearTest) 
{
    // Setup
    LCD display(8, 2);
    display.clear();

    // Verify
    char* buffer = display.buffer(); 
    
    EXPECT_EQ(0x00, buffer[0]);
    EXPECT_EQ(0x00, buffer[8]);
    EXPECT_EQ(0x00, buffer[15]);
}

TEST(LCDTestGroup, PrintCharTest) 
{
    // Setup
    LCD display(8, 2);
    display.clear();

    // Exercise
    display.print('H');
    display.print('e');
    display.print('l');
    display.print('l');
    display.print('o');
    
    // Verify
    EXPECT_STREQ("Hello", display.buffer());
}


TEST(OLEDTestGroup, ClearTest) 
{
    // Setup
    OLED display(8, 2);
    display.clear();

    // Verify
    char* buffer = display.buffer(); 
    
    EXPECT_EQ(0x00, buffer[0]);
    EXPECT_EQ(0x00, buffer[8]);
    EXPECT_EQ(0x00, buffer[15]);
}

TEST(OLEDTestGroup, PrintCharTest) 
{
    // Setup
    OLED display(8, 2);
    display.clear();

    // Exercise
    display.print('H');
    display.print('e');
    display.print('l');
    display.print('l');
    display.print('o');
    
    // Verify
    EXPECT_STREQ("Hello", display.buffer());
}

