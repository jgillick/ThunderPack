/**
 * Changes the LED strip from red to blue to green, by replacing each LED
 * sequentially down the LED strip.
 *
 * Set LED_COUNT to the number of LEDs which are in your strip.
 */

#include <stdint.h>
#include <libopencm3/stm32/rcc.h>

#include "ws2812b_conf.h"
#include "ws2812b.h"

#define LED_COUNT 16

static void pause(int duration);
static void colorWipe(uint8_t r, uint8_t g, uint8_t b);
static void clock_setup(void);

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void) {
  clock_setup();

  // Initialize the ws2812b lib
  ws2812b_init(LED_COUNT);

  // Start making colors
  while (1) {
    colorWipe(0xFF, 0x00, 0x00); // Red
    colorWipe(0x00, 0xFF, 0x00); // Green
    colorWipe(0x00, 0x00, 0xFF); // Blue
  }

  return 0;
}

/**
 * @brief Wipe a color across the entire LED string.
 */
static void colorWipe(uint8_t r, uint8_t g, uint8_t b) {
  size_t i;
  for (i = 0; i < LED_COUNT; i++) {
    ws2812b_led(i, r, g, b);
    pause(200000);
  }
}

/**
 * @brief Pause execution for an amount of time
 */
static void pause(int duration) {
  int i;
  for (i = 0; i < duration; i++) {
    __asm__("nop");
  }
}

/**
 * @brief Setup the system clock
 */
static void clock_setup() {
  struct rcc_clock_scale clockcfg = {
    .pll_source = RCC_CFGR_PLLSRC_HSI16_CLK,
    .pll_mul = RCC_CFGR_PLLMUL_MUL4,
    .pll_div = RCC_CFGR_PLLDIV_DIV2,
    .hpre = RCC_CFGR_HPRE_NODIV,
    .ppre1 = RCC_CFGR_PPRE1_NODIV,
    .ppre2 = RCC_CFGR_PPRE2_NODIV,
    .voltage_scale = PWR_SCALE1,
    .flash_waitstates = 1,
    .ahb_frequency	= 32000000,
    .apb1_frequency = 32000000,
    .apb2_frequency = 32000000,
  };
  rcc_clock_setup_pll(&clockcfg);
}