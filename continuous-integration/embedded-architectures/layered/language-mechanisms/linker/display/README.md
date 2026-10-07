# Example: Link-Time Polymorphism - Diaplay


## Build with LCD Diaplay

```bash
$ cmake -S . -B build -DUSE_IMPL=LCD
```

## Build with OLED Display

```bash
$ cmake -S . -B build -DUSE_IMPL=OLED
```

## Build with LCD Display (default)

```bash
$ cmake -S . -B build 
```

## Build with Invalid Option

```bash
$ cmake -S . -B build -DUSE_IMPL=LED
CMake Error at CMakeLists.txt:28 (message):
  Invalid value for USE_IMPL: LED.  Valid options are 'LCD' or 'OLED'.
```

