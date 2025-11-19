#include <wiringPi.h>
#include <stdio.h>

int main(void) 
{
    // Setup WiringPi using BCM GPIO numbering
    if (wiringPiSetupGpio() == -1) 
    { 
        printf("wiringPi init failed\n"); return 1; 
    }
    pinMode(17, OUTPUT);

    
    for (int i=0; i<10; i++) 
    {
        digitalWrite(17, HIGH);
        delay(500);
        digitalWrite(17, LOW);
        delay(500);
    }
    return 0;
}
