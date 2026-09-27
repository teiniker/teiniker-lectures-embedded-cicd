#include <motor_controller.h>

void MotorController::speed(uint8_t km_per_hour)
{
    _current_speed = km_per_hour;
}

uint8_t MotorController::readSpeed(void)
{
    uint8_t speed = 0;
    CanMessage message = _can->receiveMessage();
    if(message.id() == 0x33)  // Example CAN ID for speed feedback
    {
        speed = message.data()[0];
    }
    return speed;
}

void MotorController::setMotorPower(uint8_t power)
{
    CanMessage message(0x22, 1, &power);
    _can->sendMessage(message);
}

void MotorController::control(void)
{
    uint8_t actual_speed = readSpeed();
    if(actual_speed < _current_speed)
    {
        setMotorPower(100); // Full power
    }
    else if(actual_speed > _current_speed)
    {
        setMotorPower(0);   // Cut power
    }
    else
    {
        // Do nothing, maintain current power
    }
}

