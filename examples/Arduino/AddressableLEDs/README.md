# Addressable LEDs (WS2812B)
Controls a string of addressable WS2812B LEDs (like [Neopixels](https://www.adafruit.com/category/168)). This example controls 16 LEDs, which is configurable by the `LED_COUNT` value in `src/main.c`.

## Connections

From the LED strip, connect:

* Data -> `PA7`
* Power -> `Vin` or `Batt+`
* Ground -> `GND`

## WS2812B API Library
Read about how to use the provided WS2812B APIs in the [WS2812b.h](./WS2812b.h) file.

### Example
```cpp
// Initialize LED strip
#define DATA_PIN  PA7
#define LED_COUNT 16
WS2812b led_strip = WS2812b(DATA_PIN, LED_COUNT);

// Set brightness on 3 neopixels
led_strip.set(0, 0xFF, 0x00, 0x00); // LED 1 red
led_strip.set(1, 0x00, 0xFF, 0x00); // LED 2 green
led_strip.set(2, 0x00, 0x00, 0xFF); // LED 3 blue

// Send data (if you forget this line, nothing will be sent to your LED strip)
led_strip.show();
```