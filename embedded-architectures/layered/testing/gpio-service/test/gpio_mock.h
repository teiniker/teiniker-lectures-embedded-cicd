#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <gpio_interface.h>

class MockGpio : public Gpio
{
public:
    MOCK_METHOD(void, setPinMode, (uint8_t pin, PinMode mode), (override));
    MOCK_METHOD(void, writePin, (uint8_t pin, bool value), (override));
    MOCK_METHOD(bool, readPin, (uint8_t pin), (override));
    MOCK_METHOD(void, togglePin, (uint8_t pin), (override));
};
