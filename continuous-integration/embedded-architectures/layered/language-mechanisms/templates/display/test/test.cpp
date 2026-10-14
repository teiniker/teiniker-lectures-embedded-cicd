#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "lcd.h"
#include "oled.h"
#include "mock_bus.h"

using namespace std;

TEST(DisplayTestGroup, LcdClearTest) 
{
    // Setup
    LCD display;
    display.print('X');

    // Exercise
    display.clear();

    // Verify
    const char* buffer = display.buffer(); 
    EXPECT_EQ(0x00, buffer[0]);
    EXPECT_EQ(0x00, buffer[LCD::cols * LCD::rows - 1]);
}

TEST(DisplayTestGroup, LcdPrintCharTest) 
{
    // Setup
    LCD display;

    // Exercise
    for(char c : string("Hello"))
        display.print(c);
    
    // Verify
    EXPECT_STREQ("Hello", display.buffer());
}

TEST(DisplayTestGroup, OledPrintCharTest) 
{
    // Setup
    OLED display;

    // Exercise
    for(char c : string("Hello"))
        display.print(c);
    
    // Verify
    EXPECT_STREQ("Hello", display.buffer());
}

TEST(DisplayTestGroup, DisplaySizeTest) 
{
    // Verify: the configuration is part of the type
    EXPECT_EQ(16u, LCD::cols);
    EXPECT_EQ(2u, LCD::rows);
    EXPECT_EQ(21u, OLED::cols);
    EXPECT_EQ(8u, OLED::rows);
}

TEST(DisplayTestGroup, MockBusInitTest) 
{
    // Setup
    Display<MockBus, 8, 2> display;

    // Verify
    EXPECT_TRUE(display.bus().initialized);
    EXPECT_TRUE(display.bus().written.empty());
}

TEST(DisplayTestGroup, MockBusWriteTest) 
{
    // Setup
    Display<MockBus, 8, 2> display;

    // Exercise
    display.print('H');
    display.print('i');

    // Verify
    vector<uint8_t> expected = {'H', 'i'};
    EXPECT_EQ(expected, display.bus().written);
}

TEST(DisplayTestGroup, OverflowTest) 
{
    // Setup
    Display<MockBus, 2, 1> display;

    // Exercise
    display.print('A');
    display.print('B');
    display.print('C');    // beyond the display: ignored

    // Verify
    EXPECT_STREQ("AB", display.buffer());
    EXPECT_EQ(2u, display.bus().written.size());
}
