# STM32Cube

Examples written using [STM32Cube](https://www.st.com/content/st_com/en/products/embedded-software/mcu-mpu-embedded-software/stm32-embedded-software/stm32cube-mcu-mpu-packages/stm32cubef4.html#overview), which are the official hardware extraction layer (HAL) and libraries from ST.

The base configurations (clock, gpio, etc) are generated using the [STM32CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html) code generator.

## Prerequisites

### Software

 * GCC build essentials.
 * [The ARM gcc toolchain, 7.3.1 or above](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm)
 * [dfu-util](http://dfu-util.sourceforge.net/)

## Compile

To compile each example, you should only need to run `make` from within the example directory.

```bash
cd examples/STM32Cube/Blink

make
```

## Flash the program

Put the board into bootloader (DFU) mode by pressing the user button while turning the board on. Now you can run the following command to flash the program to the board:

```bash
make flash
```

This command is a wrapper for the `dfu-util` command. The full command would be:

```bash
dfu-util -d 0483:df11 -a 0 -s 0x08000000:leave -D build/program.bin
```