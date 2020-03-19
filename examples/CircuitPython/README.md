# CircuitPython / MicroPython
[CircuitPython](https://circuitpython.org/) enables Python code to run on the Thunderpack and is one of the easiest ways to get started with Microcontrollers. But first, you'll need to load the CircuitPython framework onto the board.

The CircuitPython binary for Thunderpack is included in this directory as `circuitpy-thunderpack.bin`. Follow the setup instructions to learn how to get this binary running on Thunderpack.

## Setup
First make sure you have [dfu-util](http://dfu-util.sourceforge.net/) or [STM32CubeProg](https://www.st.com/en/development-tools/stm32cubeprog.html) installed on your computer.

### Enter DFU bootloader mode
Put the board into bootloader (DFU) mode by pressing the user button while turning the board on. 

### Updating the board

#### DFU-UTIL
If you're using dfu-util, run this command (be sure the path to the `.bin` file is correct):

```bash
dfu-util -d 0483:df11 -a 0 -s 0x08000000:leave -D ./circuitpy-thunderpack.bin
```

#### STM32CubeProg
If your're using STM32Cube, open the application and use it to program the [circuitpy-thunderpack.bin](./circuitpy-thunderpack.bin) file to the board.

You can see comprehensive instructions with screenshots [here](https://learn.adafruit.com/adafruit-stm32f405-feather-express/dfu-bootloader-details).

### Using CircuitPython
If everything worked right, when you restart your board it will mount to your computer as a disk drive called "CIRCUITPY". This can take up to 30 seconds on the first boot.

To try an example, simply add the `code.py` (and the `lib` directoy, if present) file to the mounted drive and in few seconds the board will update to run the example.

Learn more at [CircuitPython](https://circuitpython.org/)
