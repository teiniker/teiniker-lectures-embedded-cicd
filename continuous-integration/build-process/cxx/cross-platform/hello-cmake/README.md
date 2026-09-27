# Example: Cross-Compiler Using CMake

Given a simple C++ program

```c++
#include <stdio.h>

int main(void)
{
	for(int i=0; i<10; i++)
	{
		printf("Hello world using CMake!\n");
	}	
	return 0;
}
```

1. We compile and link the source file into an executable using CMake:

```bash
$ cmake -DCMAKE_CXX_COMPILER=aarch64-rpi5-linux-gnu-g++ -S . -B build
$ cd build/
$ make
```

**Note:** When cross-compiling, we must explicitly specify the cross-compiler's 
name (e.g., `aarch64-rpi5-linux-gnu-g++`) to CMake using the `-DCMAKE_CXX_COMPILER` 
option on the command line. This ensures that CMake uses the correct toolchain for 
your target platform.


2. We can analyze this executable:

```bash
$ file hello 
hello: ELF 64-bit LSB executable, ARM aarch64, version 1 (SYSV), 
dynamically linked, 
interpreter /lib/ld-linux-aarch64.so.1, 
for GNU/Linux 6.16.0, 
with debug_info, 
not stripped
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
Hello world using CMake!
Hello world using CMake!
Hello world using CMake!
Hello world using CMake!
Hello world using CMake!
Hello world using CMake!
Hello world using CMake!
Hello world using CMake!
Hello world using CMake!
Hello world using CMake!
```

With this, we have created our first program using a cross compiler.

*Egon Teiniker, 2025, GPL v3.0*