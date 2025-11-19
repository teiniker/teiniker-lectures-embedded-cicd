#include <gtest/gtest.h>
#include <string>
#include <memory>

#include <controller.h>
#include "gpio_mock.h"

using namespace std;
using namespace testing;

TEST(ControllerTest, ControllerInitialize)
{
    // Setup
    MockGpio mock;
    Controller controller(&mock);

    EXPECT_CALL(mock, setPinMode(22, PinMode::INPUT));
    EXPECT_CALL(mock, setPinMode(23, PinMode::INPUT));
    EXPECT_CALL(mock, setPinMode(24, PinMode::INPUT));
    EXPECT_CALL(mock, setPinMode(5, PinMode::OUTPUT));
    EXPECT_CALL(mock, setPinMode(6, PinMode::OUTPUT));

    EXPECT_CALL(mock, writePin(5, false));
    EXPECT_CALL(mock, writePin(6, false));

    // Exercise
    controller.initialize();

    // Verify
}

TEST(ControllerTest, ControllerControl_UpButton)
{
    // Setup
    MockGpio mock;
    Controller controller(&mock);

    EXPECT_CALL(mock, readPin(22)).WillOnce(Return(true));  // UP button pressed
    EXPECT_CALL(mock, writePin(5, true));   // Motor FORWARD
    EXPECT_CALL(mock, writePin(6, false));

    // Exercise
    controller.control();

    // Verify
}

TEST(ControllerTest, ControllerControl_DownButton)
{
    // Setup
    MockGpio mock;
    Controller controller(&mock);

    EXPECT_CALL(mock, readPin(22)).WillOnce(Return(false)); // UP button not pressed
    EXPECT_CALL(mock, readPin(23)).WillOnce(Return(true));  // DOWN button pressed
    EXPECT_CALL(mock, writePin(5, false));  // Motor BACKWARD
    EXPECT_CALL(mock, writePin(6, true));

    // Exercise
    controller.control();

    // Verify
}

TEST(ControllerTest, ControllerControl_StopButton)
{
    // Setup
    MockGpio mock;
    Controller controller(&mock);

    EXPECT_CALL(mock, readPin(22)).WillOnce(Return(false)); // UP button not pressed
    EXPECT_CALL(mock, readPin(23)).WillOnce(Return(false)); // DOWN button not pressed
    EXPECT_CALL(mock, readPin(24)).WillOnce(Return(true));  // STOP button pressed
    EXPECT_CALL(mock, writePin(5, false));  // Motor STOP
    EXPECT_CALL(mock, writePin(6, false));

    // Exercise
    controller.control();

    // Verify
}
