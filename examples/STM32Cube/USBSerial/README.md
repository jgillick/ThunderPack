# Serial over USB
A simple program which demonstrates outputting serial data over USB. In this case it simply outputs the state (0/1) of the button, repeatedly, to the serial console. 

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
Once connected you should see the number `0` output, repeatedly. As you press and hold the button on the board, it should output `1`.