#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "uart_service.h"

class MockUartService : public UartService
{
public:
    MOCK_METHOD(void, init, (const BaudRate baudRate, DataBits dataBits, Parity parity, StopBits stopBits), (override));

    MOCK_METHOD(void, send, (const std::string &data), (override));
    MOCK_METHOD(std::string, receive, (), (override));

    MOCK_METHOD(uint32_t, baudRate, (), (const, override));
    MOCK_METHOD(uint8_t, dataBits, (), (const, override));
    MOCK_METHOD(uint8_t, parity, (), (const, override));
    MOCK_METHOD(uint8_t, stopBits, (), (const, override));
};
