CC=aarch64-rpi5-linux-gnu-g++
CFLAGS=-g -Wall 


all: build/hello

build:
	mkdir -p build

build/hello: build hello.cpp
	$(CC) -o build/hello hello.cpp

clean:
	rm -rf build/
