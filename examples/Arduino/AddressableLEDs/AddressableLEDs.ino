/**
 * Controls a string of addressable WS2812B LEDs, like Neopixels (https://www.adafruit.com/category/168).
 * This example controls 16 LEDs, which is configurable by the `LED_COUNT` value.
 *
 * From the LED strip, connect:
 *
 *  + Data   -> `PA6`
 *  + Power  -> `Vin` or `Batt+`
 *  + Ground -> `GND`
 *
 * To see the complete API, look at the functions in the ws2812b.h file.
 */
#include "WS2812b.h"

#define DATA_PIN  PA7
#define LED_COUNT 16

WS2812b led_strip = WS2812b(DATA_PIN, LED_COUNT);

// Fill the LED pixels one after the other with a color
void colorWipe(uint8_t r, uint8_t g, uint8_t b) {
  for (size_t i = 0; i < LED_COUNT; i++) {
      led_strip.set(i, r, g, b);
      led_strip.show();
      delay(100);
  }
}

void setup() {
}

void loop() {
 colorWipe(0xFF, 0x00, 0x00); // Red
 colorWipe(0x00, 0xFF, 0x00); // Green
 colorWipe(0x00, 0x00, 0xFF); // Blue
}