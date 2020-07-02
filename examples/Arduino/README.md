# Arduino

Arduino is a great place to start if you're just getting started with microcontrollers. It uses a simplified programming structure with C or C++.

Arduino support is provided on the ThunderPack by the [STM32duino project](https://github.com/stm32duino/Arduino_Core_STM32).

## Software Prerequisites
* [Arduino](https://www.arduino.cc/en/Main/Software)
   * Then install [STM32 Cores 1.8+](https://github.com/stm32duino/wiki/wiki/Getting-Started) in your Arduino application.
* [STM32CubeProgrammer](https://www.st.com/en/development-tools/stm32cubeprog.html)

ℹ️ Mac users: There is a known issue with installing STM32CubeProgrammer on mac. You'll likely need to install it through your terminal application. [more info](https://community.st.com/s/question/0D50X00009XkWmr/install-stm32cubeprogrammer-on-mac-osx-fails-hangs-at-step-6-of-8)


## Setup

Open your Arduino application and make the following selections under the Tools menu:

* Board: Generic STM32L0 series
* Board part number: ThunderPack
* Upload method: STM32CubeProgrammer (DFU)

<img src="https://user-images.githubusercontent.com/35894/68361520-016c4f00-00d9-11ea-856d-098a0e5d06cc.png" width="500" />

⚠️ If you do not see those options, make sure that you have [STM32 Cores 1.8+](https://github.com/stm32duino/wiki/wiki/Getting-Started) installed in your Arduino application.

## Programming

1. Open the `ino` file (i.e. Blink.ino) in your Arduino application.
2. Plug your ThunderPack into your computer with a USB cable.
3. Put the ThunderPack in bootloader mode by pressing the tactile button *while* you turn it on. (i.e. press both buttons at the same time)
4. Click Sketch > Upload

Now the program will be compiled and automatically uploaded to your ThunderPack (this might take a couple minutes). If all goes well, and no errors occur, the example program should start automatically.


# Building & Programming

[Instructions](https://github.com/jgillick/ThunderPack/wiki/Getting-Started#arduino) on how to build an example and program it to your ThunderPack with Arduino.
