#ifndef _CAN_MOCK_H_
#define _CAN_MOCK_H_

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <can_interface.h>

class MockCan : public CanInterface
{
public:
    MOCK_METHOD(void, sendMessage, (const CanMessage& message), (override));
    MOCK_METHOD(CanMessage, receiveMessage, (), (override));
};

#endif /* _CAN_MOCK_H_ */
