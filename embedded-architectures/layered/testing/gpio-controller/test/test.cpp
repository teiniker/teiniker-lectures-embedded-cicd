#include <gtest/gtest.h>
#include <string>
#include <memory>

#include <controller.h>
#include "gpio_mock.h"

using namespace std;
using namespace testing;

constexpr uint8_t BUTTON_UP_PIN = 22;
constexpr uint8_t BUTTON_DOWN_PIN = 23;
constexpr uint8_t BUTTON_STOP_PIN = 24; 

constexpr uint8_t MOTOR_IN1_PIN = 5;
constexpr uint8_t MOTOR_IN2_PIN = 6;

TEST(ControllerTest, ControllerInitialize)
{
    // Setup
    auto mock = std::make_shared<MockGpio>();
    Controller controller(mock);

    EXPECT_CALL(*mock, setPinMode(BUTTON_UP_PIN, PinMode::INPUT));
    EXPECT_CALL(*mock, setPinMode(BUTTON_DOWN_PIN, PinMode::INPUT));
    EXPECT_CALL(*mock, setPinMode(BUTTON_STOP_PIN, PinMode::INPUT));

    EXPECT_CALL(*mock, setPinMode(MOTOR_IN1_PIN, PinMode::OUTPUT));
    EXPECT_CALL(*mock, setPinMode(MOTOR_IN2_PIN, PinMode::OUTPUT));

    EXPECT_CALL(*mock, writePin(MOTOR_IN1_PIN, false));
    EXPECT_CALL(*mock, writePin(MOTOR_IN2_PIN, false));

    // Exercise
    controller.initialize();

    // Verify: Expectations are verified on destruction of mock
}

TEST(ControllerTest, ControllerControl_UpButton)
{
    // Setup
    auto mock = std::make_shared<MockGpio>();
    Controller controller(mock);

    EXPECT_CALL(*mock, readPin(BUTTON_UP_PIN)).WillOnce(Return(true));  // UP button pressed

    EXPECT_CALL(*mock, writePin(MOTOR_IN1_PIN, true));                  // Motor FORWARD
    EXPECT_CALL(*mock, writePin(MOTOR_IN2_PIN, false));

    // Exercise
    controller.control();

    // Verify: Expectations are verified on destruction of mock
}

TEST(ControllerTest, ControllerControl_DownButton)
{
    // Setup
    auto mock = std::make_shared<MockGpio>();
    Controller controller(mock);

    EXPECT_CALL(*mock, readPin(BUTTON_UP_PIN)).WillOnce(Return(false));     // UP button not pressed
    EXPECT_CALL(*mock, readPin(BUTTON_DOWN_PIN)).WillOnce(Return(true));    // DOWN button pressed

    EXPECT_CALL(*mock, writePin(MOTOR_IN1_PIN, false));                     // Motor BACKWARD
    EXPECT_CALL(*mock, writePin(MOTOR_IN2_PIN, true));

    // Exercise
    controller.control();

    // Verify: Expectations are verified on destruction of mock
}

TEST(ControllerTest, ControllerControl_StopButton)
{
    // Setup
    auto mock = std::make_shared<MockGpio>();
    Controller controller(mock);

    EXPECT_CALL(*mock, readPin(BUTTON_UP_PIN)).WillOnce(Return(false));     // UP button not pressed
    EXPECT_CALL(*mock, readPin(BUTTON_DOWN_PIN)).WillOnce(Return(false));   // DOWN button not pressed
    EXPECT_CALL(*mock, readPin(BUTTON_STOP_PIN)).WillOnce(Return(true));    // STOP button pressed

    EXPECT_CALL(*mock, writePin(MOTOR_IN1_PIN, false));                     // Motor STOP
    EXPECT_CALL(*mock, writePin(MOTOR_IN2_PIN, false));

    // Exercise
    controller.control();

    // Verify: Expectations are verified on destruction of mock
}
