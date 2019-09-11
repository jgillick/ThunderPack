# LibOpenCM3

Examples written using the [libopencm3](https://github.com/libopencm3/libopencm3) library.

The Makefiles have been borrowed/adapted from the excellent [1Bitsy](https://github.com/1Bitsy/1bitsy-examples) project.

## Prerequisites

 * GCC build essentials
 * [The ARM gcc toolchain, 7.3.1 or above](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm)
 * [dfu-util](http://dfu-util.sourceforge.net/)

## Compile

1. **Before** you can compile the examples, you must run `make` from this directory to build the libopencm3 library binaries. (you only need to run this once)
2. Now go into one of the example directories and run `make`.

**For Example:**

```bash
cd ThunderPack/examples/libopencm3

make

cd ./Blink

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