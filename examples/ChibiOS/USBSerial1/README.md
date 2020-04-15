# Serial over USB
A simple program which demonstrates sending and receiving serial data over USB.

When connected to a serial terminal, the program will print `1` ever time the button is pressed and `0` every time it is released.

If you type a number, 1 - 4, the LED for that number will be toggled on or off.

This program also uses [ChibiOS threads](http://www.chibios.org/dokuwiki/doku.php?id=chibios:howtos:createthread) to handle both receiving and sending at the same time.

## Compiling/Flashing
See the instructions [here](../README.md)

## Connecting
Once you have flashed the program, you can connect to your device with a terminal program:

* Linux
  * [GTKterm](https://elinux.org/Communicate_with_hardware_using_USB_cable_for_Ubuntu)
  * [screen](https://software.intel.com/en-us/setting-up-serial-terminal-on-system-with-linux)
* Mac
  * [Serial2](https://www.decisivetactics.com/products/serial/)
  * [CoolTerm](https://freeware.the-meiers.org/)
  * [screen](https://software.intel.com/en-us/setting-up-serial-terminal-on-system-with-linux)
* Windows
  * [Putty](https://software.intel.com/en-us/setting-up-serial-terminal-on-system-with-windows)

### Connection settings
 * Baudrate: 115200
 * Data bits: 8
 * Parity: None
 * Stop bits: 1

### Usage
Once connected you should see the number `0` output, repeatedly. As you press and hold the button on the board, it should output `1`.