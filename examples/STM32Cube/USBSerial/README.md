# Serial over USB
A simple program which demonstrates sending and receiving serial data over USB.

When connected to a serial terminal, the program will print `1` ever time the button is pressed and `0` every time it is released.

If you type a number, 1 - 4, the LED for that number will be toggled on or off.

## Compiling/Flashing
See the instructions [here](../README.md)

## Connecting
Once you have flashed the program, you can connect to your device with a terminal program:

* Linux
  * [GTKterm](https://elinux.org/Communicate_with_hardware_using_USB_cable_for_Ubuntu)
  * [screen](https://software.intel.com/en-us/setting-up-serial-terminal-on-system-with-linux)
* Mac
  * [CoolTerm](https://freeware.the-meiers.org/)
  * [Serial2](https://www.decisivetactics.com/products/serial/)
  * [screen](https://software.intel.com/en-us/setting-up-serial-terminal-on-system-with-linux)
* Windows
  * [Putty](https://software.intel.com/en-us/setting-up-serial-terminal-on-system-with-windows)

### Connection settings
 * Baudrate: 115200
 * Data bits: 8
 * Parity: None
 * Stop bits: 1

## How sending & receiving works

You can see how we send data in [main.c](./Src/main.c) via the `CDC_Transmit_FS` function, but you might be wondering which code is handling receiving data. That is handled with the `CDC_Receive_FS` function in [usbd_cdc_if.c](./Src/usbd_cdc_if.c). This function is called automatically by the USB driver every time bytes are received by the serial connection.