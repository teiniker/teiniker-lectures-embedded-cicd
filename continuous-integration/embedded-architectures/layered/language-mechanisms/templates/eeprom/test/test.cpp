#include <gtest/gtest.h>

#include "eeprom.h"

using namespace std;

TEST(EEPROMTest, AtmelVendorTest) 
{
    // Setup
    EEPROM<Atmel> eeprom; //64Kbit (8Kbit x 8) Parallel EEPROM

    // Exercise
    string vendor = eeprom.vendor();
    
    // Verify
    EXPECT_EQ("Atmel", vendor);

    // Teardown
}


TEST(EEPROMTest, MicrochipVendorTest) 
{
    // Setup
    EEPROM<Microchip> eeprom; //64Kbit (8Kbit x 8) Parallel EEPROM

    // Exercise
    string vendor = eeprom.vendor();
    
    // Verify
    EXPECT_EQ("Microchip", vendor);

    // Teardown
}


TEST(EEPROMTest, ReadWriteTest) 
{
    // Setup
    EEPROM<Microchip> eeprom; //64Kbit (8Kbit x 8) Parallel EEPROM

    // Exercise
    eeprom.write(0x0100, 0xaa);
    
    // Verify
    EXPECT_EQ(0xaa, eeprom.read(0x0100));

    // Teardown
}
