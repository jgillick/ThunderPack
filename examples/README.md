# Example Programs

The chip included on the ThunderPack board (STM32L072xx) can be programed with a variety of environments and/or libraries, each at varying levels of abstraction. For example, Arduino has the most abstraction where you don't need to understand much of the inner workings of the chip, and STM32Cube is at the opposite end of the spectrum where you're toggling specific registers on the chip itself. Often this layer of abstraction between the software and hardware is referred to as the Hardware Abstraction Layer (HAL).

I've included a series of simple examples using the following libraries/environments to get you started:

* [Arduino](./Arduino) - A common starting place for hobbyists, artists and those new to programming hardware.
* [ChibiOS](./ChibiOS) - A compact [real-time operating](https://en.wikipedia.org/wiki/Real-time_operating_system) system which includes threads & it's own hardware abstraction layer.
* [LibOpenCM3](./libopencm3) - An open source and comprehensive hardware abstraction layer.
* [STM32Cube](./STM32Cube) - Low level APIs and a very basic hardware abstraction layer provided by STM32.

Each directory has a README which explains how to setup, compile and flash the software for that library.
