#include <iostream>
#include <gtest/gtest.h>

#include "eeprom.h"

using namespace std;

TEST(EEPROMTest, PrintVendorTest) 
{
    // Setup
    EEPROM eeprom; //64Kbit (8Kbit x 8) Parallel EEPROM

    // Exercise
    cout << "EEPROM Vendor: " << eeprom.vendor() << endl;
    
    // Verify
    // Teardown
}


TEST(EEPROMTest, ReadWriteTest) 
{
    // Setup
    EEPROM eeprom; //64Kbit (8Kbit x 8) Parallel EEPROM

    // Exercise
    eeprom.write(0x0100, 0xaa);
    
    // Verify
    EXPECT_EQ(0xaa, eeprom.read(0x0100));

    // Teardown
}
