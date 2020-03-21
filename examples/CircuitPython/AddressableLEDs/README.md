# Addressable LEDs (WS2812B)
Controls a string of addressable WS2812B LEDs, like [Neopixels](https://www.adafruit.com/category/168). This example controls 16 LEDs, which is configurable by the `LED_COUNT` value in `src/main.c`.

## Connections

From the LED strip, connect:

* Data -> `PA6`
* Power -> `Vin` or `Batt+`
* Ground -> `GND`
