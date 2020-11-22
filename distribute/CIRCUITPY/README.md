ThunderPack 1.2
===============
https://github.com/jgillick/thunderpack/wiki


WARNINGS!
----------------
* ALWAYS use batteries with BUILT-IN protection circuits. _(ThunderPack board does not have any battery protection)_
* Do NOT plug the battery in backward or you could damage the board. (see instructions, below)
* Do NOT exceed the maximum rated power of the battery or components on the board.
* You CANNOT charge the battery through the VIN pin.
* ThunderPack should NOT be used as a chew toy.

ThunderPack was designed for fun and is not intended for life-supporting or business-critical applications.

Maximum Ratings
----------------
* Power Switch maximum ratings: 1A/30V (note 1)
* LED MOSFET maximum ratings:  2.3A/20V
* Onboard Voltage Regulator (AP2210N-3.3TRG1)
  * Output: 3.3V/300mA
  * Input (USB): 2.5-13.2V
* Onboard battery charger (BQ21040): 800mA/4.2V
* Power Pins:
  * 3.3v: 300mA
  * VUSB: USB host max current
  * BATT+: Battery max current
  * VIN: VUSB or BATT+ max (depending on which is powering the board)

Note 1:
You might be wondering how to use the 2.3A MOSFETs if the power switch is only rated at 1A. The MOSFETS are, themselves, switches, so you can connect an LED (or another device) directly from the BATT+ pin to any of the 4 LED pins. The main power switch will control the power to the MCU and that will control switching the MOSFETs. DO NOT connect a high current device directly to VIN, 3.3v, or VUSB.

Getting Started
----------------

### INSERTING THE BATTERY
Look at the diagram on the bottom of the board to see which direction to install the battery. Inserting it backward can damage the board.

The negative end of the battery should be on the USB-side, and the positive side should be on the power switch side of the board.


### RUNNING YOUR FIRST PROGRAM
ThunderPack comes loaded with CircuitPython and a simple test program. Turn the board on with the power switch and you should see the 4 LEDs light up in order. If you do not see this, the battery might be dead. Plug it into a USB charger and charge the battery.

When you plug the board into your computer it should mount as a CircuitPython drive device.
Flashing new firmware
If you want to continue using CircuitPython, updating the firmware is as simple as updating code.py on the USB drive which appears when you plug the board into your computer.

Otherwise, pressing the small tactile button next to the power button while you turn the board on will put it into programming mode. Now you can flash a new firmware program to the board.


### FLASHING FIRMWARE
On most systems, the dfu-util command-line tool (http://dfu-util.sourceforge.net/) will be the easiest way to get new firmware onto the board.

You can confirm that the board is in firmware programming mode by running:
dfu-util --list

To flash a new firmware program (firmware.bin) to the board, run a command like this:
dfu-util -d 0483:df11 -a 0 -s 0x08000000:leave -D ./firmware.bin


### EXAMPLES
There are many examples available for ThunderPack written with Arduino, ChibiOS, libopencm3, CircuitPython, and STM32Cube. You can browse the example library here:
https://github.com/jgillick/ThunderPack/tree/master/examples

Feel free to submit more examples to the project!
