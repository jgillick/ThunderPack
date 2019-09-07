# LED Programs

This creates a series of light/fade programs for the first LED (PA0). You can use the user button to cycle between the programs:

1. Slow fade
2. Fade fade
3. Fade with a blink
4. On

The last program used will be remembered in the EEPROM and will start again next time the device is powered on.

## Principles

This program makes use of multiple ChibiOS threads (one to manage the current program and another to listen for a button press) and uses [events](http://wiki.chibios.org/dokuwiki/doku.php?id=chibios:kb:events) to communicate between them.

## Compiling/Flashing

See the instructions [here](../README.md)