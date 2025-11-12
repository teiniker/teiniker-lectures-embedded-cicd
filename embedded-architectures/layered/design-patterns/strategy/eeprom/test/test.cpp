#include <iostream>
#include <gtest/gtest.h>

#include <eeprom_target_1.h>
#include <eeprom_target_2.h>

using namespace std;

TEST(EEPROMTest, PrintVendorTest1) 
{
    // Setup
    shared_ptr<EEPROM> eeprom = make_shared<EEPROMTarget1>(); //64Kbit (8Kbit x 8) Parallel EEPROM

    // Exercise
    cout << "EEPROM Vendor: " << eeprom->vendor() << endl;
    
    // Verify
    EXPECT_EQ("Atmel", eeprom->vendor());
    
    // Teardown
}

TEST(EEPROMTest, PrintVendorTest2) 
{
    // Setup
    shared_ptr<EEPROM> eeprom = make_shared<EEPROMTarget2>(); //128Kbit (16Kbit x 8) Serial EEPROM

    // Exercise
    cout << "EEPROM Vendor: " << eeprom->vendor() << endl;
    
    // Verify
    EXPECT_EQ("Microchip", eeprom->vendor());
    
    // Teardown
}

TEST(EEPROMTest, ReadWriteTest) 
{
    // Setup
    shared_ptr<EEPROM> eeprom = make_shared<EEPROMTarget1>(); //64Kbit (8Kbit x 8) Parallel EEPROM

    // Exercise
    eeprom->write(0x0100, 0xaa);
    
    // Verify
    EXPECT_EQ(0xaa, eeprom->read(0x0100));

    // Teardown
}
