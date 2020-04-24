# EEPROMish - Non-Volatile Memory

A simple example to demonstrate storing a value in non-volatile memory.

When the program starts, one of the LEDs will light up. Pressing the button will light the next LED, and so on and so forth. When you turn the board off and on again, the previously lit LED will be remembered and be the starting LED.

## How it works
The STM32F411 chip does not have built-in [EEPROM memory](https://en.wikipedia.org/wiki/EEPROM). However, it allows you to write to sectors of it's internal flash memory in a similar way to EEPROM. 

Each time the user presses the button to change the lit LED, the code saves this value to a byte address in the flash memory.

Inside the chip, the flash memory is devided into 11 sectors. In our case we reserved sector 1 for non-volatile memory. Before writing to a sector, the entire sector needs to be erased. To prevent the firmware from being loaded into this section, we updated the linker file ([STM32F411CEUx_FLASH_NVM.ld](./STM32F411CEUx_FLASH_NVM.ld)) to reserve sector 1 for data.

### What's the catch?
There are a few:

* It uses the same memory as the firmware, so it _might_ be erased when the firmware is updated.
* Writing to flash is slower than EEPROM.
* Flash memory might start corrupting after 10k write cycls. EEPROM can handle up to 1 million write cycles.

### Additional information
This example is pretty basic, but if you plan to write to this often, you might want to read about proper [EEPROM emulation techniques](https://www.st.com/resource/en/application_note/dm00036065-eeprom-emulation-in-stm32f40xstm32f41x-microcontrollers-stmicroelectronics.pdf).

## Compiling/Flashing
See the instructions [here](../README.md)