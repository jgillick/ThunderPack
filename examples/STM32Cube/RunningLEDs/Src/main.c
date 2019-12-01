#include "main.h"

#define SPEED     150    // How fast the LEDs run
#define LED_COUNT 4

int led_pins[] = {
  GPIO_PIN_0, // LED 1
  GPIO_PIN_1, // LED 2
  GPIO_PIN_2, // LED 3
  GPIO_PIN_3, // LED 4
};

void SystemClock_Config(void);
static void gpio_init(void);

int main(void) {
  // Init the system
  HAL_Init();
  SystemClock_Config();

  // Setup GPIO
  gpio_init();

  uint8_t led_idx = 0;
  while (1) {
    // Toggle the current LED
    HAL_GPIO_TogglePin(GPIOA, led_pins[led_idx]);
    HAL_Delay(SPEED);

    // Increment LED
    led_idx++;
    if (led_idx >= LED_COUNT) {
      led_idx = 0;
    }
  }
}

/**
  * System Clock Configuration
  */
void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  // Configure the main internal regulator output voltage
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  // Initializes the CPU, AHB and APB busses clocks
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    Error_Handler();
  }

  // Initializes the CPU, AHB and APB busses clocks
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
    Error_Handler();
  }
}

/**
  * GPIO Initialization Function
  */
static void gpio_init(void) {
  GPIO_InitTypeDef GPIO_InitStruct = {};

  __HAL_RCC_GPIOA_CLK_ENABLE();

  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

  // Enabled LED outputs
  uint8_t i;
  for (i = 0; i < LED_COUNT; i++) {
    HAL_GPIO_WritePin(GPIOA, led_pins[i], GPIO_PIN_RESET);
    GPIO_InitStruct.Pin = led_pins[i];
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  }
}

/**
  * This function is executed in case of error occurrence.
  */
void Error_Handler(void) {
}
