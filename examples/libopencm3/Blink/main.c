/*
  Continuously blinks the first LED (PA0). When the user button
  is pressed, the LED blinks faster.
*/

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#define LONG_PAUSE  100000
#define SHORT_PAUSE 50000

static void gpio_setup(void) {
  // set LED1 (PA0) to output
  rcc_periph_clock_enable(RCC_GPIOA);
  gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO0);

  // Button (PB4) to input, open drain
  rcc_periph_clock_enable(RCC_GPIOB);
  gpio_mode_setup(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO4);
}

// Pause execution for an amount of time
static void pause(int duration) {
  int i;
  for (i = 0; i < duration; i++) {
    __asm__("nop");
  }
}

int main(void) {
  gpio_setup();

  int i;
  while (1) {
    gpio_toggle(GPIOA, GPIO0);

    /* Upon button press, blink more slowly. */
    if (gpio_get(GPIOB, GPIO4)) {
      pause(SHORT_PAUSE);
    } else {
      pause(LONG_PAUSE);
    }
  }

  return 0;
}