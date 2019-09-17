# Arduino

Arduino support will _hopefully_ be coming via the [STM32duino project](https://github.com/stm32duino/Arduino_Core_STM32). I currently have a [fork](https://github.com/jgillick/Arduino_Core_STM32) of that repository with support for Thunder Pack, which will hopefully get merged into the main project in an upcoming pull request.

## Install
Until I get a pull request for Thunder Pack merged into the main project, here's how to get it working:

1. Add [STM32 Cores](https://github.com/stm32duino/wiki/wiki/Getting-Started) to your arduino application.
2. Find where the STM32 Cores files were added on your computer: https://github.com/stm32duino/wiki/wiki/Where-are-sources#stm32-core-sources-files-location
3. Clone this fork of the STM32duino project to your computer: https://github.com/jgillick/Arduino_Core_STM32
4. Replace the STM32 Cores `boards.txt` & `platform.txt` files (in the directory from step #2) with the one from the forked repo.
5. Add the `variants/THUNDER_PACK` from the forked repo to the `variants` directory in the STM32 Cores area (in the directory from step #2).
6. Restart the Arduino application.

# Compile & Flash

1. From within the Arduino IDE select the "Thunder Pack" from Tools > Board.
2. Put Thunder Pack in bootloader mode by pressing the User Button while powering it on.
3. Now select Sketch > Upload to compile and flash the program to your device.
