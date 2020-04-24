#include "stm32f4xx_hal.h"
#include "thunderpack.h"

#define MAX_PWM   0xFFF
#define PWM_INC   45
#define PWM_SPEED 10

TIM_HandleTypeDef htim2;

static void timer_init(void);
static void gpio_init(void);

int main(void) {
  // Init the system
  HAL_Init();
  thunderpack_clock_init();

  // Setup/start PWM
  timer_init();
  gpio_init();
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
  * Setup PWM timer
  */
static void timer_init(void) {
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  __HAL_RCC_TIM2_CLK_ENABLE();

  // Timer
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = MAX_PWM;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  HAL_TIM_PWM_Init(&htim2);

  // Sync
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig);

  // PWM
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_ENABLE;
  HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1);
}

/**
 * Setup GPIO pins
 */
static void gpio_init(void) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  __HAL_RCC_GPIOA_CLK_ENABLE();

  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}