#include <stdlib.h>
#include <string.h>
#include "PinNames.h"
#include "pinmap.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_ll_gpio.h"
#include "./WS2812b.h"

// System clock divider to get the number of ticks for a 800kHz data rate
#define MAGIC_800_INT   310000

// The white LED value is present when the white and red order offset are different
#define HAS_WHITE       (wOffset != rOffset)

#pragma GCC push_options
#pragma GCC optimize ("Os")

/**
 * Create an instance of the WS2812b LED driver
 */
WS2812b::WS2812b(uint32_t data_pin, size_t led_num, uint8_t color_order)
: led_num(led_num)
{
  port = get_GPIO_Port(STM_PORT(data_pin));
  pin = STM_LL_GPIO_PIN(STM_PIN(data_pin));

  init_gpio();
  calculate_data_order(color_order);
  create_buffer();
}

WS2812b::WS2812b(GPIO_TypeDef *data_pin_port, uint32_t data_pin, size_t led_num, uint8_t color_order)
: port(data_pin_port),
  pin(data_pin),
  led_num(led_num)
{
  init_gpio();
  calculate_data_order(color_order);
  create_buffer();
}

WS2812b::~WS2812b() {
  free(buffer);
}

/**
 * Setup the buffer
 */
void WS2812b::create_buffer() {
  free(buffer);

  if (led_num) {
    buffer_size = led_num;
    if (HAS_WHITE) {
      buffer_size *= 4;
    } else {
      buffer_size *= 3;
    }
    buffer = (uint8_t*) malloc(buffer_size * sizeof(uint8_t));
    fill(0x00, 0x00, 0x00, 0x00);
  }
}

/**
 * Set the RGB[W] color offset for the LED protocol.
 * Copied from: https://github.com/adafruit/Adafruit_NeoPixel
 */
void WS2812b::calculate_data_order(uint8_t order) {
  wOffset = (order >> 6) & 0b11;
  rOffset = (order >> 4) & 0b11;
  gOffset = (order >> 2) & 0b11;
  bOffset =  order       & 0b11;
}

/**
 * Initialize the data pin
 */
void WS2812b::init_gpio() {
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = pin | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(port, &GPIO_InitStruct);
}

/**
 * Set the color on one of the LEds in the buffer
 * using a 32-bit color (0xFFFFFF = RRGGBB[WW])
 */
void WS2812b::set(uint32_t led, uint32_t color) {
  uint8_t red, green, blue, white;

  // Extract colors
  if (HAS_WHITE) {
    red = (color >> 24) & 0xFF;
    green = (color >> 16) & 0xFF;
    blue = (color >> 8) & 0xFF;
    white = (color >> 0) & 0xFF;
  } else {
    red = (color >> 16) & 0xFF;
    green = (color >> 8) & 0xFF;
    blue = (color >> 0) & 0xFF;
    white = 0;
  }

  set(led, red, green, blue, white);
}

/**
 * Set the color on one of the LEds in the buffer
 * using three (or four) 8-bit colors
 */
void WS2812b::set(uint32_t led, uint8_t red, uint8_t green, uint8_t blue, uint8_t white) {
  uint8_t start = 0;

  if (led >= led_num || led_num == 0) {
    return;
  }

  if (HAS_WHITE) {
    start = led * 4;
  } else {
    start = led * 3;
  }

  // Set colors to buffer
  buffer[start + rOffset] = red;
  buffer[start + gOffset] = green;
  buffer[start + bOffset] = blue;
  if (HAS_WHITE) {
    buffer[start + wOffset] = white;
  }
}

/**
 * Set all LEDs to this color using a 32-bit color (0xFFFFFF = RRGGBB)
 */
void WS2812b::fill(uint32_t color) {
  uint8_t red, green, blue, white;

  // Extract colors
  if (HAS_WHITE) {
    red = (color >> 24) & 0xFF;
    green = (color >> 16) & 0xFF;
    blue = (color >> 8) & 0xFF;
    white = (color >> 0) & 0xFF;
  } else {
    red = (color >> 16) & 0xFF;
    green = (color >> 8) & 0xFF;
    blue = (color >> 1) & 0xFF;
    white = 0;
  }

  fill(red, green, blue, white);
}

/**
 * Set all LEDs to this color using
 * using three (or four) 8-bit colors
 */
void WS2812b::fill(uint8_t red, uint8_t green, uint8_t blue, uint8_t white) {
  for (uint32_t i = 0; i < led_num; i++) {
    set(i, red, green, blue, white);
  }
}

/**
 * Return the 32-b-t RGB color defined for an LED
 */
uint32_t WS2812b::get(uint32_t led) {
  if (led >= led_num) {
    return 0;
  }
  return buffer[led];
}

/**
 * Send all the LED data out on the data pin.
 */
void WS2812b::show() {
  wait_for_reset();
  send_data();
  start_reset();
}

/**
 * Write data for a single color out to the data pin.
 */
void WS2812b::send_data() {
  size_t i = 0;
  uint8_t mask = 0x80;
  uint32_t pulse = 0;

  // Calculate protocol timing
  //  + 800kHz
  //  + digital 1 is ~70% duty cycle
  //  + digital 0 is ~30% duty cycle
  uint32_t sys_freq = SystemCoreClock;
  uint32_t interval = sys_freq/MAGIC_800_INT;
  uint32_t pulse1 = interval * .7;
  uint32_t pulse0 = interval * .3;

  // Enable DWT in debug core to count cycles
  // Useable when interrupts disabled, as opposed to Systick->VAL
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

  // Disable interrupts
  __disable_irq();

  for(;;) {
    pulse = (buffer[i] & mask) ? pulse1 : pulse0;
    DWT->CYCCNT = 0;

    // Set high
    LL_GPIO_SetOutputPin(port, pin);
    while(DWT->CYCCNT < pulse);

    // Set low
    LL_GPIO_ResetOutputPin(port, pin);
    while(DWT->CYCCNT < interval);

    // End of color byte
    if(!(mask >>= 1)) {
      // Next pixel
      mask = 0x80;
      // We're at the end of the buffer
      if(++i >= buffer_size) break;
    }
  }

  // Enable interrupts again
  __enable_irq();
}

/**
 * Calculate the SYSTICK time when the reset pulse will be complete,
 * if started now.
 */
void WS2812b::start_reset() {
  uint32_t ticks_per_us = SystemCoreClock / 1000 / 1000;
  next_start_ms = HAL_GetTick();
  next_start_us = (SysTick->VAL / ticks_per_us) + 100;
  if (next_start_us > 1000) {
    next_start_ms++;
    next_start_us = next_start_us - 1000;
  }
}

/**
 * Wait until the reset pulse is complete.
 */
void WS2812b::wait_for_reset() {
  uint32_t ticks_per_us = SystemCoreClock / 1000 / 1000;
  while (HAL_GetTick() < next_start_ms && SysTick->VAL / ticks_per_us < next_start_us) {}
}