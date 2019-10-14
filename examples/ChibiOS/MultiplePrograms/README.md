# LED Programs

This is a more advanced example which creates a series of lighting programs on LED 1 (PA0). You can use the user button to cycle between the programs. The last program used will be remembered in the EEPROM and will start again next time the device is powered on.

**Programs:**

1. Slow fade
2. Fade fade
3. Fade with a blink
4. On 100%

## Principles

### Multiple Threads
This program makes use of multiple ChibiOS threads: one to manage the current program and another to listen for a button press. Then it uses [events](http://wiki.chibios.org/dokuwiki/doku.php?id=chibios:kb:events) to communicate between them.

### Exponential PWM Fading
The PWM value is incremented exponentially to provide a more natural fade. [More info](https://diarmuid.ie/blog/pwm-exponential-led-fading-on-arduino-or-other-platforms/).

### EEPROM
ChibiOS doesn't have built-in EEPROM functions. So, following the [EEPROM example](../EEPROM/), this program uses the EEPROM functions from the STM32Cube library.

## Compiling/Flashing
See the instructions [here](../README.md)