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
#include "ws2812b_conf.h"
#include "ws2812b.h"

#define LED_COUNT 16


// Fill the dots one after the other with a color
void colorWipe(uint8_t r, uint8_t g, uint8_t b) {
  size_t i;
  for (i = 0; i < LED_COUNT; i++) {
      ws2812b_led(i, r, g, b);
      delay(50);
  }
}

void setup() {
  ws2812b_init(LED_COUNT);
}

void loop() {
  colorWipe(0xFF, 0x00, 0x00); // Red
  colorWipe(0x00, 0xFF, 0x00); // Green
  colorWipe(0x00, 0x00, 0xFF); // Blue
}
