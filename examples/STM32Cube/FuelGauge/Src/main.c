#include "main.h"

#define BIT_PER_VOLT (0b111111111111 / 3.3)

// Determine the 4 gauge ADC levels
// A lithium battery range is 2.5v - 4.2v.
// After the voltage divider the range is: 1.25 - 2.1
int adc_levels[] = {
  1.20 * BIT_PER_VOLT,
  1.725 * BIT_PER_VOLT,
  1.85 * BIT_PER_VOLT,
  1.90 * BIT_PER_VOLT,
};

int led_pins[] = {
  GPIO_PIN_0, // LED 1
  GPIO_PIN_1, // LED 2
  GPIO_PIN_2, // LED 3
  GPIO_PIN_3, // LED 4
};

ADC_HandleTypeDef hadc;

void SystemClock_Config(void);
static void gpio_init(void);
static void adc_init(void);

int main(void) {
  // Init the system
  HAL_Init();
  SystemClock_Config();

  // Setup GPIO
  gpio_init();
  adc_init();

  uint8_t i;
  uint32_t adc_val;
  HAL_ADC_Start(&hadc);
  while (1) {
    if (HAL_ADC_PollForConversion(&hadc, 100) == HAL_OK) {
      adc_val = HAL_ADC_GetValue(&hadc);

      // Set LEDs
      for (i = 0; i < 4; i++) {
        if (adc_val >= adc_levels[i]) {
          HAL_GPIO_WritePin(GPIOA, led_pins[i], GPIO_PIN_SET);
        } else {
          HAL_GPIO_WritePin(GPIOA, led_pins[i], GPIO_PIN_RESET);
        }
      }
    }
    HAL_Delay(100);
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
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/**
 * Configure Analog to Digital
 */
static void adc_init(void){
  ADC_ChannelConfTypeDef sConfig = {0};

  // Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  hadc.Instance = ADC1;
  hadc.Init.OversamplingMode = DISABLE;
  hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV1;
  hadc.Init.Resolution = ADC_RESOLUTION_12B;
  hadc.Init.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  hadc.Init.ScanConvMode = DISABLE;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ContinuousConvMode = ENABLE;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc.Init.DMAContinuousRequests = DISABLE;
  hadc.Init.EOCSelection = DISABLE;
  hadc.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
  hadc.Init.LowPowerAutoWait = DISABLE;
  hadc.Init.LowPowerFrequencyMode = DISABLE;
  hadc.Init.LowPowerAutoPowerOff = DISABLE;
  if (HAL_ADC_Init(&hadc) != HAL_OK) {
    Error_Handler();
  }

  // Configure for the selected ADC regular channel to be converted.
  sConfig.Channel = ADC_CHANNEL_7;
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK) {
    Error_Handler();
  }
}

/**
  * This function is executed in case of error occurrence.
  */
void Error_Handler(void) {
}
