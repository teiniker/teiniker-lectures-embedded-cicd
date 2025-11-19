# Example: Blink LED 

```c++
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
```

To build the example, you can either call the compiler manually:

```bash
$ gcc -Wall -o blink blink.c -lwiringPi
$ ./blink
```

or with the help of CMake:
```bash
$ cmake -S . -B build
$ cd build/
$ make
$ ./blink
```