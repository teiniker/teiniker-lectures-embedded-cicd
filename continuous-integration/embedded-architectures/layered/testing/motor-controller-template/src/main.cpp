#include <can_bus.h>
#include <motor_controller.h>

int main(void)
{
    CanBus bus;
    MotorController<CanBus> controller(bus);    // Production instantiation

    controller.speed(50);   // Set desired speed to 50 km/h
    controller.control();

    return 0;
}
