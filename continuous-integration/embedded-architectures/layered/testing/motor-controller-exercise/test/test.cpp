#include <gtest/gtest.h>
#include <string>
#include <memory>

#include <motor_controller.h>
#include "can_mock.h"

using namespace std;
using namespace testing;

TEST(MotorControllerTest, MotorControllerSpeedUp)
{
    // TODO: Setup

    // Exercise
    controller.speed(50);  // Set desired speed to 50 km/h
    controller.control();

    // Verify: Expectations are verified on destruction of mock
}

TEST(MotorControllerTest, MotorControllerSpeedDown)
{
    // TODO: Setup

    // Exercise
    controller.speed(50);  // Set desired speed to 50 km/h
    controller.control();

    // Verify: Expectations are verified on destruction of mock
}


TEST(MotorControllerTest, MotorControllerSpeedConstant)
{
    // TODO: Setup

    // Exercise
    controller.speed(50);  // Set desired speed to 50 km/h
    controller.control();

    // Verify: Expectations are verified on destruction of mock
}
