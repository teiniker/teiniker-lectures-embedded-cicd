# gpiod Tools

These tools come from **libgpiod**, the official Linux GPIO character-device interface.

## Setup 

```bash
$ sudo apt install gpiod
```

## Commands 

| Command      | What it does                                  |
| ------------ | --------------------------------------------- |
| `gpiodetect` | Lists available gpiochips (e.g., `gpiochip0`) |
| `gpioinfo`   | Shows all GPIO lines, names, direction, users |
| `gpioset`    | Set pin(s) high or low                        |
| `gpioget`    | Read pin value(s)                             |
| `gpiomon`    | Monitor pin edges (interrupt-like)            |


_Example_: Set GPIO17 high
```bash
$ gpioset gpiochip0 17=1
```

_Example_: Set GPIO17 low
```bash
$ gpioset gpiochip0 17=0
```

_Example_: Read GPIO27
```bash
$ gpioget gpiochip0 27
```

_Example_: Monitor rising/falling edges
```bash
$ gpiomon gpiochip0 27
```


## References

* [libgpiod: Command-line tools](https://libgpiod.readthedocs.io/en/latest/gpio_tools.html)


_Egon Teiniker, 2025, GPL v3.0_