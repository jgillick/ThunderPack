#include "stm32f4xx_hal.h"
#include "thunderpack.h"

#define SPEED     150    // How fast the LEDs run
#define LED_COUNT 4

int led_pins[] = {
  GPIO_PIN_0, // LED 1
  GPIO_PIN_1, // LED 2
  GPIO_PIN_2, // LED 3
  GPIO_PIN_3, // LED 4
};

static void gpio_init(void);

int main(void) {
  // Init the system
  HAL_Init();
  thunderpack_clock_init();

  // Setup GPIO
  gpio_init();

  uint8_t led_idx = 0;
  while (1) {
    // Toggle the current LED
    HAL_GPIO_TogglePin(GPIOA, led_pins[led_idx]);
    HAL_Delay(SPEED);

    // Increment LED
    led_idx++;
    led_idx = led_idx % LED_COUNT; // wrap around back to 0 when we get to the end
  }
}

/**
  * GPIO Initialization Function
  */
static void gpio_init(void) {
  GPIO_InitTypeDef GPIO_InitStruct = {};

  __HAL_RCC_GPIOA_CLK_ENABLE();

  // Enabled LED outputs
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GPIO_PIN_RESET);
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/**
  * This function is executed in case of error occurrence.
  */
void Error_Handler(void) {
}
