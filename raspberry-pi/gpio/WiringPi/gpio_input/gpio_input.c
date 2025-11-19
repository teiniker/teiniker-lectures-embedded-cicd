#include <wiringPi.h> // Include WiringPi library!
#include <stdio.h>

// Circuit: GPIO27 --[Button]--> GND

const int PIN_BUTTON = 27;

int main(void)
{
  // uses BCM numbering of the GPIOs and directly accesses the GPIO registers.
  wiringPiSetupGpio();

  // set pin to input
  // pin mode ..(INPUT, OUTPUT, PWM_OUTPUT, GPIO_CLOCK)
  pinMode(PIN_BUTTON, INPUT);

  // pull up/down mode (PUD_OFF, PUD_UP, PUD_DOWN) => down
  pullUpDnControl(PIN_BUTTON, PUD_UP);

  while(1)
  {
      int value = digitalRead(PIN_BUTTON);
    
      if(value == LOW) 
      {
          printf("ON\n");
      }
      else
      {
          printf("OFF\n");
      }
      delay(100);
  }
  return 0;
}