# EEPROMish - Non-Volatile Memory

A simple example to demonstrate storing a value in non-volatile memory.

When the program starts, one of the LEDs will light up. Pressing the button will light the next LED, and so on and so forth. When you turn the board off and on again, the previously lit LED will be remembered and be the starting LED.

## Setup
Copy `code.py` to the CIRCUITPY drive, when Thunderpack mounts to your computer.

## How it works
The STM32F411 chip does not have built-in [EEPROM memory](https://en.wikipedia.org/wiki/EEPROM). However, it allows you to write to sectors of it's internal flash memory in a similar way to EEPROM. 

Each time the user presses the button to change the lit LED, the code saves this value to a byte address in the flash memory.

Lucky for you, most of the complexities of using flash memory as emulated EEPROM is handled for you by the CircuitPython library.

### What's the catch?

* Writing to flash is slower than EEPROM.
* Flash memory might start corrupting after 10k write cycls. EEPROM can handle up to 1 million write cycles.
