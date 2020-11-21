# Example Programs

Thunder Pack can be programed with a variety of languages, libraries and frameworks; each at varying levels of abstraction. CircuitPython and Arduino, for example, have the highest level of abstraction and you don't need to understand much of the inner workings of the chip to get it to do some complex tasks. STM32Cube is at the opposite end of the spectrum where you're working much closer to the chip level. Often this layer of abstraction between the software and hardware is referred to as the Hardware Abstraction Layer (HAL). If you've never programmed microcontrollers before, CircuitPython or Arduino is a great place to start.

I've included a series of simple examples across the following libraries/environments to get you started:

* [CircuitPython](./CircuitPython) - Easily program the board with the Python language. By far the easiest way to start.
* [Arduino](./Arduino) - A common starting place for hobbyists, artists and those new to programming hardware.
* [ChibiOS](./ChibiOS) - A compact [real-time operating system](https://en.wikipedia.org/wiki/Real-time_operating_system) which includes threads & it's own hardware abstraction layer.
* [LibOpenCM3](./libopencm3) - An open source and comprehensive hardware abstraction layer in C and C++.
* [STM32Cube](./STM32Cube) - The default hardware abstraction layer provided by STMicroelectronics and contains quite a few lower level APIs.

Each directory has a README file which explains how to setup, compile and flash the software for that library.
