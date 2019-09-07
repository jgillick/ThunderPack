# Thunder Pack

An ARM microcontroller with built-in battery and power management system, 
all in a compact package that can fit in your pocket. Originally designed for compact personal/wearable projects and lighting.

Why? After stringing together countless portable LED controllers that consisted of wiring a flat lipo battery to a charge/boost controller to a arduino nano, I wanted something better. I always found those earlier versions to be janky, fragile and setup for the single purpose they were built for.

More details: https://hackaday.io/project/161054-lit-fist

## Features
* Ultra-low-power 32-bit microcontroller ([STM32L072xB](https://www.st.com/resource/en/datasheet/stm32l072v8.pdf))
* Integrated high-capacity 18650 lithium battery.
* Built-in USB charger.
* USB bootloader.
* 4 high-current PWM outputs (2.3A per channel)
* On-board LEDs for each PWM output for debugging.
* Tactile on/off switch
* Tactile user button.
* Breadboard friendly.
* Access to 14 GPIO pins.