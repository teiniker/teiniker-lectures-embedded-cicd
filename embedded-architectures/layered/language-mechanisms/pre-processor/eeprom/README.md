# Example: Conditional Compilation 

If we want to use the same functionality on different embedded systems, we 
often have to **exchange parts of the code within a function**. 

This can be achieved using **pre-processor directives** for conditional compilation.

* `#ifdef`: **If defined** - Checks whether a macro is defined.

* `#ifndef`: **If not defined** - The opposite of `#ifdef`.

* `#if`: **If expression is true** - Allows numeric or boolean-style expressions.
    Can use operators: `==`, `!=`, `>`, `<`, `>=`, `<=`, `&&`, `||`.

* `#else`: **If previous condition fails** - Works with `#ifdef`, `#ifndef`, or `#if`.

* `#endif`: **Closes the conditional block** - Every `#if`, `#ifdef`, and 
    `#ifndef` must end with a matching `#endif`.


_Example_: Conditional compilation - Compiles code only if `FEATURE_ENABLED` has been defined.
```C++
#ifdef FEATURE_ENABLED
    printf("Feature is ON\n");
#endif
```

This is equivalent to:
```C++
#if defined(FEATURE_ENABLED)
```

_Example_: AVR Arduino Core - [HardwareSerial.h](https://github.com/arduino/ArduinoCore-avr/blob/master/cores/arduino/HardwareSerial.h)

```C++
#if defined(UBRRH) || defined(UBRR0H)
  extern HardwareSerial Serial;
  #define HAVE_HWSERIAL0
#endif
#if defined(UBRR1H)
  extern HardwareSerial Serial1;
  #define HAVE_HWSERIAL1
#endif
#if defined(UBRR2H)
  extern HardwareSerial Serial2;
  #define HAVE_HWSERIAL2
#endif
#if defined(UBRR3H)
  extern HardwareSerial Serial3;
  #define HAVE_HWSERIAL3
#endif
```

## Consequences of Conditional Compilation

**Advantages**:
- Keeps related code together, easy to see all variants in one place.
- Simple to implement for trivial platform-specific differences.

**Disadvantages**:
- Adds `#ifdef` clutter and **reduces readability** as differences grow.
- Prevents compiling multiple variants simultaneously for testing.
- Larger platform-specific implementations become **hard to maintain**.

**Recommendation**: Use in-source `#if` guards for **small, simple differences**.
Prefer separate implementation files and link-time polymorphism for larger or 
multiple-platform implementations to keep code clear and testable.


## References

* [Designing a HAL in C++](https://blog.mbedded.ninja/programming/languages/c-plus-plus/designing-a-hal-in-cpp/)

_Egon Teiniker, 2025, GPL v3.0_
