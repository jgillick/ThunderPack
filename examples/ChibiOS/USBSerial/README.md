# Serial over USB
A simple program which demonstrates 2-way serial communication over USB by echoing back any character you send to it.

## Compiling/Flashing
See the instructions [here](../README.md)

## Connecting
Once you have flashed the program, you can connect to your device with a terminal program:

* Linux
  * [GTKterm](https://elinux.org/Communicate_with_hardware_using_USB_cable_for_Ubuntu)
  * [screen](https://software.intel.com/en-us/setting-up-serial-terminal-on-system-with-linux)
* Mac
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
Once you connect, you should see the message: "USB connection ready! Type a character: ". Now type any character and the program will repeat it back to you on the terminal.