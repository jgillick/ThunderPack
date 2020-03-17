#include "main.h"
#include "thunderpack.h"

#define MAX_PWM   0xFFF
#define PWM_INC   45
#define PWM_SPEED 10

TIM_HandleTypeDef htim2;

static void MX_TIM2_Init(void);

int main(void) {
  // Init the system
  HAL_Init();
  init_system_clock();

  // Setup/start PWM
  MX_TIM2_Init();
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);

  int8_t direction = 1;
  uint32_t fadeVal = 0;
  while (1) {
    // Set the LED PWM value
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, fadeVal);

    // Increment and reverse if we've reached the limits
    fadeVal += PWM_INC * direction;
    if (fadeVal <= 0 || fadeVal >= MAX_PWM) {
      direction *= -1;
    }

    // A brief pause
    HAL_Delay(PWM_SPEED);
  }
}

/**
  * TIM2 Initialization Function
  */
static void MX_TIM2_Init(void) {
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 4095;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  HAL_TIM_Base_Init(&htim2);

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig);
  HAL_TIM_PWM_Init(&htim2);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig);

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 4095;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_ENABLE;
  HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1);

  HAL_TIM_MspPostInit(&htim2);
}

