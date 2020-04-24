#include <stdint.h>;

/**
 * Implements a basic library for sending data to various WS2812b (neopixel) chips.
 * The code is heavily based on the Neopixel CircuitPython library:
 *  - https://github.com/adafruit/Adafruit_CircuitPython_NeoPixel
 *  - https://github.com/adafruit/circuitpython/blob/master/ports/stm/common-hal/neopixel_write/__init__.c
 *
 * USAGE
 * -----
 *  // Initialize LED strip
 *  #define DATA_PIN  PA7
 *  #define LED_COUNT 16
 *  WS2812b led_strip = WS2812b(DATA_PIN, LED_COUNT);
 *
 *  // Set colors
 *  led_strip.set(0, 0xFF, 0x00, 0x00); // LED 1 red
 *  led_strip.set(1, 0x00, 0xFF, 0x00); // LED 2 green
 *  led_strip.set(2, 0x00, 0x00, 0xFF); // LED 3 blue
 *
 *  // Send data (if you forget this line, nothing will be sent to your LEDs)
 *  led_strip.show();
 */

#ifndef WS2812B_H
#define WS2812B_H

///////
// Color order permutations copied from
// https://github.com/adafruit/Adafruit_NeoPixel/blob/master/Adafruit_NeoPixel.h
///////

// RGB NeoPixel permutations; white and red offsets are always same
// Offset:         W        R        G        B
#define COLORS_RGB  ((0<<6) | (0<<4) | (1<<2) | (2)) ///< Transmit as R,G,B
#define COLORS_GRB  ((1<<6) | (1<<4) | (0<<2) | (2)) ///< Transmit as G,R,B

// RGBW NeoPixel permutations; all 4 offsets are distinct
// Offset:         W          R          G          B
#define COLORS_RGBW ((3<<6) | (0<<4) | (1<<2) | (2)) ///< Transmit as R,G,B,W
#define COLORS_GRBW ((3<<6) | (1<<4) | (0<<2) | (2)) ///< Transmit as G,R,B,W


class WS2812b {
public:
  /**
   * Create an instance of the WS2812b LED driver
   * @param data_pin - Arduino pin number the LEDs are connected to (i.e. D7).
   * @param led_num - Number of LEDs
   * @param color_order - One of the COLORS_* constants used to define the R,G,B[,W] color order
   *                      the data protocol requires.
   */
  WS2812b(uint32_t data_pin, size_t led_num, uint8_t color_order=COLORS_GRB);

  /**
   * Create an instance of the WS2812b LED driver with chip port & pin.
   * @param data_pin_port - Chip port of te data pin (i.e. GPIOA)
   * @param data_pin_port - Chip pin to send data on (i.e. PA6)
   * @param led_num - Number of LEDs
   * @param color_order - One of the COLORS_* constants used to define the R,G,B[,W] color order
   *                      the data protocol requires.
   */
  WS2812b(GPIO_TypeDef *data_pin_port, uint32_t data_pin, size_t led_num, uint8_t color_order=COLORS_GRB);

  /**
   * Cleanup buffer and memory.
   */
  ~WS2812b();

  /**
   * Set the color on one of the LEds in the buffer
   * using a 32-bit color (0xFFFFFF = RRGGBB[WW])
   *
   * @param led - The LED index to set
   * @param color - The color represented as a hex value of RRGGBBWW
   */
  void set(uint32_t led, uint32_t color);

  /**
   * Set the color on one of the LEds in the buffer
   * using individual 8-bit color values.
   *
   * @param led - The LED index to set
   * @param red - The red brightness
   * @param blue - The blue brightness
   * @param green - The green brightness
   * @param white - The white brightness
   */
  void set(uint32_t led, uint8_t red, uint8_t green, uint8_t blue, uint8_t white=0);

  /**
   * Set all LEDs to the same color
   * @param color - The color represented as a hex value of RRGGBBWW
   */
  void fill(uint32_t color);

  /**
   * Set all LEDs to the same color
   *
   * @param red - The red brightness
   * @param blue - The blue brightness
   * @param green - The green brightness
   * @param white - The white brightness
   */
  void fill(uint8_t red, uint8_t green, uint8_t blue, uint8_t white=0);

  /**
   * Get the color assigned to an LED.
   * @param led - The LED index to get
   * @return A 32-bit color representing RGB[W]
   */
  uint32_t get(uint32_t led);

  /**
   * Send the LED data out to the LEDs through the data pin.
   */
  void show();

private:
  size_t led_num;
  size_t buffer_size;
  uint8_t *buffer;
  uint8_t wOffset = 0;
  uint8_t rOffset = 2;
  uint8_t gOffset = 1;
  uint8_t bOffset = 3;
  uint32_t next_start_ms = 0;
  uint32_t next_start_us = 0;
  uint32_t pin;
  GPIO_TypeDef *port;

  void init_gpio();
  void calculate_data_order(uint8_t color_order);
  void create_buffer();
  void send_data();
  void start_reset();
  void wait_for_reset();
};

#endif
