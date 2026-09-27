#include <controller.h>

void Controller::initialize(void)
{
    _gpio->setPinMode(22, PinMode::INPUT);    // Button: UP
    _gpio->setPinMode(23, PinMode::INPUT);    // Button: DOWN
    _gpio->setPinMode(24, PinMode::INPUT);    // Button: STOP

    _gpio->setPinMode(5, PinMode::OUTPUT);    // Motor IN1
    _gpio->setPinMode(6, PinMode::OUTPUT);    // Motor IN2

    // Ensure motor is stopped at initialization
    _gpio->writePin(5, false);
    _gpio->writePin(6, false);
}


void Controller::control(void)
{
    Button currentButton = readButton();

    switch(currentButton)
    {
        case Button::UP:
            setMotor(MotorState::FORWARD);
            break;
     
        case Button::DOWN:
            setMotor(MotorState::BACKWARD);
            break;
        
        case Button::STOP:
            setMotor(MotorState::STOP);
            break;
    }
}


Button Controller::readButton(void)
{
    if (_gpio->readPin(22))
    {
        return Button::UP;
    }
    else if (_gpio->readPin(23))
    {
        return Button::DOWN;
    }
    else if (_gpio->readPin(24))
    {
        return Button::STOP;
    }
    else
    {
        return Button::STOP;
    }
}

void Controller::setMotor(MotorState state)
{
    switch(state)
    {
        case MotorState::FORWARD:
            _gpio->writePin(5, true);
            _gpio->writePin(6, false);
            break;
     
        case MotorState::BACKWARD:
            _gpio->writePin(5, false);
            _gpio->writePin(6, true);
            break;
        
        case MotorState::STOP:
            _gpio->writePin(5, false);
            _gpio->writePin(6, false);
            break;
    }
}

