# Example: Cross-Compiler 

Given a simple C++ program

```c++
#include <stdio.h>

int main(void)
{
	for(int i=0; i<10; i++)
	{
		printf("Hello world!\n");
	}	
	return 0;
}
```

1. We compile and link the source file into an executable:

```bash
$ mkdir build
$ aarch64-rpi5-linux-gnu-g++ -o build/hello hello.cpp
```

2. We can analyze this executable:

```bash
$ file build/hello 
build/hello: ELF 64-bit LSB executable, ARM aarch64, version 1 (SYSV), 
    dynamically linked, interpreter /lib/ld-linux-aarch64.so.1, 
    for GNU/Linux 6.16.0, with debug_info, not stripped
```

3. Copy the executable to a Raspberry Pi and execute it

In the simplest case, we copy the executable to the Raspberry Pi using scp:
```bash
$ scp build/hello student@xxx.xxx.xxx.xxx:/home/student/Downloads
```

Afterwards, we can open a terminal via SSH and execute the executable:

```bash
$ ssh student@xxx.xxx.xxx.xxx

student@raspberrypi:~ $ cd Downloads/
student@raspberrypi:~/Downloads $ ./hello 
Hello world!
Hello world!
Hello world!
Hello world!
Hello world!
Hello world!
Hello world!
Hello world!
Hello world!
Hello world!
```

With this, we have created our first program using a cross compiler.

*Egon Teiniker, 2025, GPL v3.0*