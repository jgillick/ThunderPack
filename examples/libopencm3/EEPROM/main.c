/**
 * Light up the next LED in sequence each time you press the button.
 * When you turn the board off and on again, the last LED will be remembered and be
 * the starting LED in the sequence.
 */

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/flash.h>

// Save the LED index to the first byte address in the EEPROM
#define LED_INDEX_ADDR  0x08080000

/**
 * Light up the LED by index
 */
static void lightLED(uint8_t index) {
  // Reset LEDs
  gpio_clear(GPIOA, GPIO0);
  gpio_clear(GPIOA, GPIO1);
  gpio_clear(GPIOA, GPIO2);
  gpio_clear(GPIOA, GPIO3);

  // Light the selected LED
  switch(index) {
    case 3:
      gpio_set(GPIOA, GPIO3);
      break;
    case 2:
      gpio_set(GPIOA, GPIO2);
      break;
    case 1:
      gpio_set(GPIOA, GPIO1);
      break;
    default:
      gpio_set(GPIOA, GPIO0);
  }
}

static void gpioSetup(void) {
  // set LED1 - LED4 (PA0 - PA03) to output
  rcc_periph_clock_enable(RCC_GPIOA);
  gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO0);
  gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO1);
  gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO2);
  gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO3);

  // Button (PB4) to input, open drain
  rcc_periph_clock_enable(RCC_GPIOB);
  gpio_mode_setup(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO4);
}

int main(void) {
  gpioSetup();

  // Read the LED index from EEPROM and light it up
  uint8_t ledIndex = *(uint8_t*) LED_INDEX_ADDR;
  lightLED(ledIndex);

  // Change the lit LED when the button is pressed
  uint8_t buttonState = 0;
  uint8_t lastButtonState = 0;
  while (1) {
    buttonState = gpio_get(GPIOB, GPIO4);

    // Update the LED index when the button WAS pressed, but is no longer.
    if (!buttonState && lastButtonState) {
      ledIndex++;
      if (ledIndex > 3) {
        ledIndex = 0;
      }

      // Light up the new LED
      lightLED(ledIndex);

      // Save LED index to EEPROM
      eeprom_program_word(LED_INDEX_ADDR, ledIndex);
    }

    lastButtonState = buttonState;
  }

  return 0;
}