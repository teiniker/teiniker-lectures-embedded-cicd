#include <gtest/gtest.h>

#include "can.h"
#include "can_impl.h"
#include "can_proxy.h"

using namespace std;

shared_ptr<CanInterface> create_can_interface() 
{
    auto service = make_shared<CanImpl>();
    auto proxy = make_shared<CanProxy>(service);
    return proxy;
}

TEST(CanProxyTest, SendAndReceiveMessage) 
{
    auto service = create_can_interface();
     uint8_t data[3] = {0x01, 0x02, 0x03};
    CanMessage message(0x100, 3, data);

    service->sendMessage(message);
    CanMessage received_message = service->receiveMessage();

    EXPECT_EQ(0x100, received_message.id());
    EXPECT_EQ(3, received_message.length());
    EXPECT_EQ(0x01, received_message.data()[0]);
    EXPECT_EQ(0x02, received_message.data()[1]);
    EXPECT_EQ(0x03, received_message.data()[2]);
    
    EXPECT_EQ(message, received_message);
}

TEST(CanProxyTest, SendMessageInvalidId) 
{
    auto service = create_can_interface();
    uint8_t data[2] = {0x0A, 0x0B};
 
    CanMessage message(0x800, 2, data); // Invalid ID (> 0x7FF)

    EXPECT_THROW(service->sendMessage(message), invalid_argument);
}

TEST(CanProxyTest, SendMessageInvalidLength) 
{
    auto service = create_can_interface();
    uint8_t data[9] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
 
    CanMessage message(0x100, 9, data); // Invalid length (> 8)

    EXPECT_THROW(service->sendMessage(message), invalid_argument);
}


