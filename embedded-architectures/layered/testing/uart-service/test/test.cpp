#include <gtest/gtest.h>
#include <string>
#include <memory>

#include <uart_service.h>
#include "uart_mock.h"

using namespace std;
using testing::Return;

TEST(UartServiceTest, SetUartParameters)
{
    // Setup
    MockUartService mock;
    // gMock requires expectations to be set before the mock functions are called, otherwise the behavior is undefined.
    EXPECT_CALL(mock, init(BaudRate::BR_115200, DataBits::EIGHT, Parity::NONE, StopBits::ONE));
    EXPECT_CALL(mock, baudRate()).WillOnce(Return(115200));
    EXPECT_CALL(mock, dataBits()).WillOnce(Return(8));
    EXPECT_CALL(mock, parity()).WillOnce(Return(0));
    EXPECT_CALL(mock, stopBits()).WillOnce(Return(1));

    // Exercise
    mock.init(BaudRate::BR_115200, DataBits::EIGHT, Parity::NONE, StopBits::ONE);

    // Verify
    EXPECT_EQ(115200, mock.baudRate());
    EXPECT_EQ(8, mock.dataBits());
    EXPECT_EQ(0, mock.parity());
    EXPECT_EQ(1, mock.stopBits());
}

TEST(UartServiceTest, UartSendReceive_Mock)
{
    // Setup
    MockUartService mock;
    string testData = "Hello World!";

    EXPECT_CALL(mock, send(testData)).Times(1);
    EXPECT_CALL(mock, receive())
        .Times(1)
        .WillOnce(Return(testData));

    // Exercise
    mock.send(testData);
    string receivedData = mock.receive();

    // Verify
    EXPECT_EQ(testData, receivedData);
}

