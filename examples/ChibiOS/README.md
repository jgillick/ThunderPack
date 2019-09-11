# ChibiOS Examples

These examples are written using the [ChibiOS RTOS](http://chibios.org/dokuwiki/doku.php). 

## Prerequisites

 * GCC build essentials
 * [The ARM gcc toolchain, 7.3.1 or above](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm)
 * [dfu-util](http://dfu-util.sourceforge.net/)
 * ChibiOS Supporting Files (see below)

## ChibiOS Supporting Files

In order to compile the examples, you'll need to download the ChibiOS 18.2.x files to a directory on your computer:
http://chibios.org/dokuwiki/doku.php?id=chibios:downloads:start

Then you'll need to set a global environment variable to that directory (this variable needs to be accessible by the Makefile). 

For example, in a bash environment you would set the variable in `~/.base_profile`

```bash
export CHIBIOS="${HOME}/Documents/ChibiOS_18.2.1"
```

## Compiling

Now that you have all the prerequisites out of the way, you can compile and flash the board:

```bash
make
```

## Flash the program

Put the board into bootloader (DFU) mode by pressing the user button while turning the board on. Now you can run the following command to flash the program to the board:

```bash
dfu-util -d 0483:df11 -a 0 -s 0x08000000:leave -D build/program.bin
```

Or you can simply run:

```bash
make flash-dfu
```