/*
  Turns on the LED (PA0). When the user button (PB4) is pressed.
*/

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include "thunderpack.h"

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

	while (1) {
    if (gpio_get(GPIOB, GPIO4)) {
      gpio_set(GPIOA, GPIO0);
    } else {
      gpio_clear(GPIOA, GPIO0);
    }
	}

	return 0;
}