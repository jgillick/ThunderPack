/*
  Continuously blinks the first LED (PA0). When the user button
  is pressed, the LED blinks faster.
*/

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include "thunderpack.h"

#define LED_COUNT 4

int led_pins[] = {
  GPIO0, // LED 1
  GPIO1, // LED 2
  GPIO2, // LED 3
  GPIO3, // LED 4
};

static void gpio_setup(void);

int main(void) {
  thunderpack_clock_init();
  gpio_setup();

  uint8_t led_idx = 0;
  while (1) {
    // Toggle the current LED
    gpio_toggle(GPIOA, led_pins[led_idx]);
    delay(150);

    // Increment LED
    led_idx++;
    if (led_idx >= LED_COUNT) {
      led_idx = 0;
    }
  }

  return 0;
}

static void gpio_setup(void) {
  rcc_periph_clock_enable(RCC_GPIOA);

  // Enabled LED outputs
  uint8_t i;
  for (i = 0; i < LED_COUNT; i++) {
    gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, led_pins[i]);
  }
}
