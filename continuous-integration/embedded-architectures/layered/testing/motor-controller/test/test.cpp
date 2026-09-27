#include <gtest/gtest.h>
#include <string>
#include <memory>

#include <motor_controller.h>
#include "can_mock.h"

using namespace std;
using namespace testing;

TEST(MotorControllerTest, MotorControllerSpeedUp)
{
    // Setup
    auto mock = std::make_shared<MockCan>();
    MotorController controller(mock);

    uint8_t speed_data[1] = {0};
    uint8_t motor_data[1] = {100};

    CanMessage speed_msg(0x33, 1, speed_data);  // Simulated speed 0 km/h
    CanMessage motor_msg(0x22, 1, motor_data);  // Expected motor power command to 100%

    EXPECT_CALL(*mock, receiveMessage()).WillOnce(Return(speed_msg));
    EXPECT_CALL(*mock, sendMessage(motor_msg)).Times(1);

    // Exercise
    controller.speed(50);  // Set desired speed to 50 km/h
    controller.control();

    // Verify: Expectations are verified on destruction of mock
}

TEST(MotorControllerTest, MotorControllerSpeedDown)
{
    // Setup
    auto mock = std::make_shared<MockCan>();
    MotorController controller(mock);

    uint8_t speed_data[1] = {70};
    uint8_t motor_data[1] = {0};

    CanMessage speed_msg(0x33, 1, speed_data);  // Simulated speed 70 km/h
    CanMessage motor_msg(0x22, 1, motor_data);  // Expected motor power command to 0%

    EXPECT_CALL(*mock, receiveMessage()).WillOnce(Return(speed_msg));
    EXPECT_CALL(*mock, sendMessage(motor_msg)).Times(1);

    // Exercise
    controller.speed(50);  // Set desired speed to 50 km/h
    controller.control();

    // Verify: Expectations are verified on destruction of mock
}


TEST(MotorControllerTest, MotorControllerSpeedConstant)
{
    // Setup
    auto mock = std::make_shared<MockCan>();
    MotorController controller(mock);

    uint8_t speed_data[1] = {50};
    uint8_t motor_data[1] = {0};

    CanMessage speed_msg(0x33, 1, speed_data);  // Simulated speed 50 km/h
    CanMessage motor_msg(0x22, 1, motor_data);  // Expected motor power command to 0%

    EXPECT_CALL(*mock, receiveMessage()).WillOnce(Return(speed_msg));
    EXPECT_CALL(*mock, sendMessage(motor_msg)).Times(0);    // Message is not sent

    // Exercise
    controller.speed(50);  // Set desired speed to 50 km/h
    controller.control();

    // Verify: Expectations are verified on destruction of mock
}
