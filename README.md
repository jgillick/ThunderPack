# Thunder Pack

<img src="https://cdn.hackaday.io/images/5175201571795557825.JPG" alt="ThunderPack Board" width="500" />

<a href="https://www.tindie.com/stores/jeremy/?ref=offsite_badges&utm_source=sellers_jeremy&utm_medium=badges&utm_campaign=badge_large"><img src="https://d2ss6ovg47m0r5.cloudfront.net/badges/tindie-larges.png" alt="I sell on Tindie" width="200" height="104"></a>

A kick-ass microcontroller board with everything you need in a compact package that fits in your pocket. Integrated battery, power management system, Arm microcontroller, USB bootloader, 4 high-power PWM outputs, and 12 GPIOs.

Why? After years of wiring together portable LED controllers that all consisted of a lithum batter, charge controller, power switch, and an arduino nano (or similar), I wanted something better. I always found those versions to be janky and fragile -- not to mention concerned that damaging the pouch cell battery might make it explode! This board has everything I need with room to expand.

* [Overview](https://github.com/jgillick/ThunderPack/wiki/)
* [Getting started](https://github.com/jgillick/ThunderPack/wiki/Getting-Started)
* [Pins & functions](https://github.com/jgillick/ThunderPack/wiki/Pinout-Details)
* [History & build log](https://hackaday.io/project/161054-lit-fist)

## Features

* 32-bit microcontroller with frequency up to 100Mhz ([STM32F411xx](https://www.st.com/resource/en/datasheet/stm32f411ce.pdf))
* High-capacity 18650 lithium battery with integrated 500mA charger
* 4 high-current PWM outputs (2.3A per channel!) with an LED on each for debugging.
* Additional 12 GPIOs (USART, I2C, SPI, USB)
* USB bootloader
* Tactile on/off switch
* Tactile user button with built-in debounce circuit
* Breadboard friendly
* Supports Arduino & CircuitPython.
* Durable [AF](https://www.urbandictionary.com/define.php?term=af)! (Put a tube of heat shrink around it for a super easy case if you'd like.)

<img src="./images/breadboard.jpg" alt="On the breadboard" width="400" />

_Breadboard friendly_

## Board & Pinout

<img src="./images/pinout.svg" alt="Pinout diagram" />

[See the full list of pins and details.](https://github.com/jgillick/ThunderPack/wiki/Pinout-Details)
