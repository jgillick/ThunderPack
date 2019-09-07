# Blink

The ubiquitous "Hello World" of hardware examples. This simply blinks the first LED (PA0). Pressing the button will blink the LED faster.

## Principles
This program uses [virtual timers](http://www.chibios.org/dokuwiki/doku.php?id=chibios:book:kernel_vt) to blink the LED without blocking the main loop.

## Compiling/Flashing
See the instructions [here](../README.md)