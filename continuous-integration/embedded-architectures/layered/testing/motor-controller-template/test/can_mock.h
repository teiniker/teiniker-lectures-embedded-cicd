#ifndef _CAN_MOCK_H_
#define _CAN_MOCK_H_

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <can_message.h>

// No base class: MockCan only needs the same methods as CanBus
class MockCan
{
public:
    MOCK_METHOD(void, sendMessage, (const CanMessage& message));
    MOCK_METHOD(CanMessage, receiveMessage, ());
};

#endif /* _CAN_MOCK_H_ */
