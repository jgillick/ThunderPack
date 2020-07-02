# CircuitPython / MicroPython
[CircuitPython](https://circuitpython.org/) enables Python code to run on the Thunderpack and is one of the easiest ways to get started with Microcontrollers. But first, you'll need to load the CircuitPython framework onto the board.

You can download the CircuitPython runtime binary for Thunderpack from here:
https://circuitpython.org/board/thunderpack/

Then use the following setup instructions to learn how to get this binary running on Thunderpack.

## Initial Setup
First make sure you have either [dfu-util](http://dfu-util.sourceforge.net/) or [STM32CubeProg](https://www.st.com/en/development-tools/stm32cubeprog.html) installed on your computer.

### Enter DFU bootloader mode
Put the ThunderPack in bootloader mode by pressing the tactile button *while* you turn it on. (i.e. press both buttons at the same time)

### Updating the board

#### DFU-UTIL
If you're using dfu-util, run this command (be sure the path to the `.bin` file is correct):

```bash
dfu-util -d 0483:df11 -a 0 -s 0x08000000:leave -D ./adafruit-circuitpython-thunderpack-en_US-5.2.0.bin
```

#### STM32CubeProg
If your're using STM32Cube, open the application and use it to program [the binary](https://circuitpython.org/board/thunderpack/) to the board.

You can see comprehensive instructions with screenshots [here](https://learn.adafruit.com/adafruit-stm32f405-feather-express/dfu-bootloader-details).

## Using CircuitPython
If everything worked right, when you restart your board it will mount to your computer as a disk drive called "CIRCUITPY". This can take up to 30 seconds on the first boot.

To try an example, simply add the `code.py`file (and the `lib` directoy, if present) to the mounted drive and in few seconds the board will update to run it.

Learn more at [CircuitPython](https://circuitpython.org/)
