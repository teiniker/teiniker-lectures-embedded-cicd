#include <gtest/gtest.h>

#include "can.h"
#include "can_impl.h"

using namespace std;

TEST(CanServiceTest, SendMessage) 
{
    auto service = CanImpl();
    uint8_t data[3] = {0x01, 0x02, 0x03};
    CanMessage message(0x100, 3, data);

    service.sendMessage(message);
    CanMessage received_message = service.receiveMessage();

    EXPECT_EQ(0x100, received_message.id());
    EXPECT_EQ(3, received_message.length());
    EXPECT_EQ(0x01, received_message.data()[0]);
    EXPECT_EQ(0x02, received_message.data()[1]);
    EXPECT_EQ(0x03, received_message.data()[2]);
    
    EXPECT_EQ(message, received_message);
}
