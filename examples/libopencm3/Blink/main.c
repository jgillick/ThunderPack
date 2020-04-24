/*
  Continuously blinks the first LED (PA0). When the user button
  is pressed, the LED blinks faster.
*/

#include <stdint.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include "thunderpack.h"

#define SPEED_SLOW 800
#define SPEED_FAST 200

static void gpio_setup(void) {
  // set LED1 (PA0) to output
  rcc_periph_clock_enable(RCC_GPIOA);
  gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO0);

  // Button (PB4) to input, open drain
  rcc_periph_clock_enable(RCC_GPIOB);
  gpio_mode_setup(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO4);
}

int main(void) {
  thunderpack_clock_init();
  gpio_setup();

  int i;
  while (1) {
    gpio_toggle(GPIOA, GPIO0);

    // Upon button press, blink more slowly.
    if (gpio_get(GPIOB, GPIO4)) {
      delay(SPEED_SLOW);
    } else {
      delay(SPEED_FAST);
    }
  }

  return 0;
}