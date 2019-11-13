# Addressable LEDs (WS2812B)
Controls a string of addressable WS2812B LEDs, like [Neopixels](https://www.adafruit.com/category/168). This example controls 16 LEDs, which is configurable by the `LED_COUNT` value in `main.c`.

## Connections

From the LED strip, connect:

* Data -> `PA6`
* Power -> `Vin` or `Batt+`
* Ground -> `GND`

## Compiling/Flashing
See the instructions [here](../README.md)

## WS2812B API Library
Read about how to use the provided WS2812B APIs in the [ws2812b.h](./Src/ws2812b.h) file.

The API & implementation is based on [this tutorial](https://github.com/MaJerle/STM32_WS2812B_TIM_PWM_DMA). It uses the DMA controller and a timer's PWM output to send the WS2812B data in a fast and processor efficient manner.