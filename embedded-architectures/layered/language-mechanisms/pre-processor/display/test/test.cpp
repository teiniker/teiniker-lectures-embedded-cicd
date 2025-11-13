#include <gtest/gtest.h>

#include <display.h>

TEST(DisplayTestGroup, ClearTest) 
{
    // Setup
    Display display(8, 2);
    display.clear();

    // Verify
    char* buffer = display.buffer(); 
    
    EXPECT_EQ(0x00, buffer[0]);
    EXPECT_EQ(0x00, buffer[8]);
    EXPECT_EQ(0x00, buffer[15]);
}

TEST(DisplayTestGroup, PrintCharTest) 
{
    // Setup
    Display display(8, 2);
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

