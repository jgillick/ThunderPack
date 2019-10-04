# Thunder Pack

An ARM microcontroller with built-in battery and power management system, all in a compact package that fits in your pocket.

Why? After stringing together countless portable LED controllers that consisted of wiring a flat lipo battery to a charge controller to a arduino nano, I wanted something better. I always found those earlier versions to be janky, fragile and setup for the single purpose they were built for.

More details: https://hackaday.io/project/161054-lit-fist

## Features

* Ultra-low-power 32-bit microcontroller ([STM32L072xx](https://www.st.com/resource/en/datasheet/stm32l072v8.pdf))
* Integrated high-capacity 18650 lithium battery
* 4 high-current PWM outputs (2.3A per channel!) with an LED on each for debugging.
* 12 GPIOs, USART, I2C, SPI, USB
* USB bootloader
* Tactile on/off switch
* Tactile user button with built-in debounce circuit
* Breadboard friendly