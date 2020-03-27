#include <stdlib.h>
#include <math.h>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_dma.h"
#include "ws2812b.h"

/**
 * @brief The number of bytes used per LED
 */
#if USE_RGBW
#define BYTES_PER_LED           4
#else /* USE_RGBW */
#define BYTES_PER_LED           3
#endif /* !USE_RGBW */

/**
 * @brief The number of bits used per LED (8 bits per byte)
 */
#define BITS_PER_LED  (BYTES_PER_LED * 8)

/**
 * @brief Number of LEDs to put in the buffer.
 */
#define LEDS_IN_BUFFER 2

/**
 * @brief Number of bits in the PWM buffer used by the DMA controller.
 */
#define PWM_BUFFER_SIZE  (LEDS_IN_BUFFER * BITS_PER_LED)

/**
 * @brief Return if a bit value is set.
 */
#define IS_BIT_SET(VAL, BIT_IDX)  (VAL & (1 << BIT_IDX))

/**
 * Output state values
 */
typedef enum output_states {
  OUTPUT_IDLE,
  OUTPUT_DATA,
  OUTPUT_RESET,
} output_states_t;

/**
 * @brief The number of LEDs in the strip
 */
static uint8_t led_count = 1;

/**
 * @brief Array of 3x (or 4x) number of leds (R, G, B[, W] colors)
 */
static uint8_t *led_colors;

/**
 * @brief Buffer holding the next two LED colors, as an array of PWM values to represent each bit.
 */
static uint16_t pwm_led_data[PWM_BUFFER_SIZE];

/**
 * @brief The current state of the output.
 */
static volatile output_states_t output_state = OUTPUT_IDLE;

/**
 * @brief The LEDs need another update, after the current update has completed.
 * This is usually when some LEDs are updated during the current update cycle.
 */
static uint8_t needs_update;

/**
 * @brief The index of the current LED we're processing the PWM values for.
 */
static uint32_t current_led;

/**
 * @brief The binary 1 PWM dury cycle value (set in timer_init)
 */
static uint16_t binary_1;

/**
 * @brief The binary 0 PWM dury cycle value (set in timer_init)
 */
static uint16_t binary_0;

/**
 * @brief Counts how many reset pulses have been sent.
 */
static uint8_t reset_counter;

/**
 * @brief Timer config init object
 */
TIM_HandleTypeDef htim;

/**
 * @brief DMA config init object
 */
DMA_HandleTypeDef hdma_tim;

void        start_output(uint32_t dataLen);
void        gpio_init(void);
void        timer_init(void);
void        dma_init(void);
void        stop_output(void);
void        should_update(size_t led);
static void led_fill_led_pwm_data(size_t ledIdx, uint16_t* ptr);
static void led_update_sequence(uint8_t tc);
static void dma_half_transfer_handler(TIM_HandleTypeDef *htim);
static void dma_transfer_complete_handler(TIM_HandleTypeDef *htim);

static void led_start_reset_pulse();
#if USE_RGBW
void        ws2812b_led_internal(size_t index, uint8_t r, uint8_t g, uint8_t b, uint8_t w);
#else
void        ws2812b_led_internal(size_t index, uint8_t r, uint8_t g, uint8_t b);
#endif
void        ws2812b_led_rgb_internal(size_t index, uint32_t rgbw);


/**
 * @brief  Initialize the ws2823b library and peripherals.
 * @param  num_leds  The number of LEDs in the LED strip.
 */
void ws2812b_init(size_t num_leds) {
  // Reserve memory
  led_count = num_leds;
  led_colors = (uint8_t *)malloc(BYTES_PER_LED * led_count);

  // Initialize Peripherals
  gpio_init();
  timer_init();
  dma_init();

  // Set all LEDs to off
#if USE_RGBW
  ws2812b_led_all(0, 0, 0, 0);
#else
  ws2812b_led_all(0, 0, 0);
#endif
}


/**
 * @brief  Set R,G,B color for specific LED
 * @param  index  LED index in array, starting from `0`
 * @param  r      Red color value
 * @param  g      Green color value
 * @param  b      Blue color value
 * @param  [w]    White color value
 * @return Non
 */
void ws2812b_led(size_t index, uint8_t r, uint8_t g, uint8_t b
#if USE_RGBW
, uint8_t w) {
  ws2812b_led_internal(index, r, g, b, w);
#else
) {
  ws2812b_led_internal(index, r, g, b);
#endif /* USE_RGBW */

  should_update(index);
}

/**
 * @brief Set the same RGB value on all LEDs
 * @param  r      Red color value
 * @param  g      Green color value
 * @param  b      Blue color value
 * @param  [w]    White color value
 * @return None
 */
void ws2812b_led_all(uint8_t r, uint8_t g, uint8_t b
#if USE_RGBW
, uint8_t w
#endif /* USE_RGBW */
) {
  size_t index;
  for (index = 0; index < led_count; index++) {
#if USE_RGBW
    ws2812b_led_internal(index, r, g, b, w);
#else
    ws2812b_led_internal(index, r, g, b);
#endif /* USE_RGBW */
  }
  should_update(0);
}

/**
 * @brief Set LED color using a single 16 or 32-bit value to represent RGB[W].
 *
 * RGB: 24-bit:  0xFFFFFF = RRGGBB = 255, 255, 255
 * RGBW: 32-bit: 0xFFFFFFFF = RRGGBBWW = 255, 255, 255, 255
 *
 * @param  index  LED index in array, starting from `0`
 * @param  rgbw   Color value
 */
void ws2812b_led_rgb(size_t index, uint32_t rgbw) {
  ws2812b_led_rgb_internal(index, rgbw);
  should_update(index);
}

/**
 * @brief Set the same color on all LEDs with a single 16 or 32-bit value to represent RGB[W]
 * @see   ws2812b_led_rgb()
 * @param  rgbw   Color value
 */
void ws2812b_led_all_rgb(uint32_t rgbw) {
  size_t index;
  for (index = 0; index < led_count; index++) {
    ws2812b_led_rgb_internal(index, rgbw);
  }
  should_update(0);
}

/**
 * @brief Internal version of ws2812b_led which does not automatically call should_update
 */
void ws2812b_led_internal(size_t index, uint8_t r, uint8_t g, uint8_t b
#if USE_RGBW
, uint8_t w
#endif /* USE_RGBW */
) {
  if (index < led_count) {
    led_colors[index * BYTES_PER_LED + 0] = r;
    led_colors[index * BYTES_PER_LED + 1] = g;
    led_colors[index * BYTES_PER_LED + 2] = b;
#if USE_RGBW
    led_colors[index * BYTES_PER_LED + 3] = w;
#endif /* USE_RGBW */
  }
}

/**
 * @brief Internal version of ws2812b_led_rgb which does not automatically call should_update
 */
void ws2812b_led_rgb_internal(size_t index, uint32_t rgbw) {
  if (index < led_count) {
#if USE_RGBW
    led_colors[index * BYTES_PER_LED + 0] = (rgbw >> 24) & 0xFF; // Red
    led_colors[index * BYTES_PER_LED + 1] = (rgbw >> 16) & 0xFF; // Green
    led_colors[index * BYTES_PER_LED + 2] = (rgbw >> 8) & 0xFF;  // Blue
    led_colors[index * BYTES_PER_LED + 3] = (rgbw >> 0) & 0xFF;  // White
#else
    led_colors[index * BYTES_PER_LED + 0] = (rgbw >> 16) & 0xFF; // Red
    led_colors[index * BYTES_PER_LED + 1] = (rgbw >> 8) & 0xFF;  // Green
    led_colors[index * BYTES_PER_LED + 2] = (rgbw >> 0) & 0xFF;  // Blue
#endif /* USE_RGBW */
  }
}

/**
 * @brief Setup DMA/TIM to send LED data.
 */
void start_output(uint32_t dataLen) {
  stop_output();
  HAL_TIM_PWM_Start_DMA(&htim, TIMER_CHANNEL, (uint32_t *)pwm_led_data, dataLen);
}

/**
 * @brief  Stop the reset data DMA & Timer output.
 */
void stop_output() {
  HAL_TIM_PWM_Stop_DMA(&htim, TIMER_CHANNEL);

  // Required, in case we are starting output before we return from the callback
  __HAL_UNLOCK(&hdma_tim);
  hdma_tim.State = HAL_DMA_STATE_READY;
}

/**
 * @brief Setup the GPIO
 */
void gpio_init() {
  GPIO_InitTypeDef GPIO_PWMStruct = {0};

  if (DATA_PORT == GPIOA) __HAL_RCC_GPIOA_CLK_ENABLE();
  else if (DATA_PORT == GPIOB) __HAL_RCC_GPIOB_CLK_ENABLE();
  else if (DATA_PORT == GPIOC) __HAL_RCC_GPIOC_CLK_ENABLE();
#ifdef GPIOD
  else if (DATA_PORT == GPIOD) __HAL_RCC_GPIOD_CLK_ENABLE();
#endif
#ifdef GPIOE
  else if (DATA_PORT == GPIOE) __HAL_RCC_GPIOE_CLK_ENABLE();
#endif
#ifdef GPIOF
  else if (DATA_PORT == GPIOF) __HAL_RCC_GPIOF_CLK_ENABLE();
#endif
#ifdef GPIOG
  else if (DATA_PORT == GPIOG) __HAL_RCC_GPIOG_CLK_ENABLE();
#endif
#ifdef GPIOH
  else if (DATA_PORT == GPIOH) __HAL_RCC_GPIOH_CLK_ENABLE();
#endif
#ifdef GPIOI
  else if (DATA_PORT == GPIOI) __HAL_RCC_GPIOI_CLK_ENABLE();
#endif
#ifdef GPIOJ
  else if (DATA_PORT == GPIOJ) __HAL_RCC_GPIOJ_CLK_ENABLE();
#endif
#ifdef GPIOK
  else if (DATA_PORT == GPIOK) __HAL_RCC_GPIOk_CLK_ENABLE();
#endif

  GPIO_PWMStruct.Pin = DATA_PIN;
  GPIO_PWMStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_PWMStruct.Pull = GPIO_NOPULL;
  GPIO_PWMStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_PWMStruct.Alternate = DATA_PIN_AF;
  HAL_GPIO_Init(DATA_PORT, &GPIO_PWMStruct);

  // GPIO_InitTypeDef GPIO_InitStruct = {0};
  // HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_8, GPIO_PIN_RESET);
  // GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_8;
  // GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  // GPIO_InitStruct.Pull = GPIO_NOPULL;
  // GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  // HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  // HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_8, GPIO_PIN_SET);
}

/**
 * @brief Setup the timer to output 800kHz
 */
void timer_init() {
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  // Enable
  SET_BIT(RCC->APB1ENR, TIMER_RCC_BIT);
  htim.Instance = TIMER;
  htim.Init.Prescaler = 0;
  htim.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim.Init.Period = (SystemCoreClock / SIGNAL_HZ) - 1;
  htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  HAL_TIM_PWM_Init(&htim);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim, &sMasterConfig);

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  HAL_TIM_PWM_ConfigChannel(&htim, &sConfigOC, TIMER_CHANNEL);

  // Save the binary 1 & 0 PWM values so we don't need to calculate them at runtime
  binary_1 = ceil(htim.Init.Period * BINARY_1_DUTY_MULTIPLIER);
  binary_0 = floor(htim.Init.Period * BINARY_0_DUTY_MULTIPLIER);
}

/**
 * @brief Setup the DMA to Timer
 */
void dma_init() {
  if (DMA == DMA1) {
    __HAL_RCC_DMA1_CLK_ENABLE();  
  } else if (DMA == DMA2) {
    __HAL_RCC_DMA2_CLK_ENABLE();
  }

#ifdef DMA_STREAM
  hdma_tim.Instance = DMA_STREAM;
  hdma_tim.Init.Channel = DMA_CHANNEL;
#else
  hdma_tim.Instance = DMA_CHANNEL;
  hdma_tim.Init.Request = DMA_REQUEST;
#endif 
  hdma_tim.Init.Direction = DMA_MEMORY_TO_PERIPH;
  hdma_tim.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_tim.Init.MemInc = DMA_MINC_ENABLE;
  hdma_tim.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
  hdma_tim.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
  hdma_tim.Init.Mode = DMA_CIRCULAR;
  hdma_tim.Init.Priority = DMA_PRIORITY_VERY_HIGH;
#ifdef DMA_FIFOMODE_DISABLE
  hdma_tim.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
#endif

  HAL_DMA_Init(&hdma_tim);
  // LL_DMA_DisableDoubleBufferMode()
  
  uint32_t dmaIdCC;
  switch (TIMER_CHANNEL) {
    case TIM_CHANNEL_1:
      dmaIdCC = TIM_DMA_ID_CC1;
      break;
    case TIM_CHANNEL_2:
      dmaIdCC = TIM_DMA_ID_CC2;
      break;
    case TIM_CHANNEL_3:
      dmaIdCC = TIM_DMA_ID_CC3;
      break;
    case TIM_CHANNEL_4:
      dmaIdCC = TIM_DMA_ID_CC4;
      break;
    default:
      return;
  }
  __HAL_LINKDMA(&htim, hdma[dmaIdCC], hdma_tim);

  HAL_TIM_RegisterCallback(&htim, HAL_TIM_PWM_PULSE_FINISHED_CB_ID, dma_transfer_complete_handler);
  HAL_TIM_RegisterCallback(&htim, HAL_TIM_PWM_PULSE_FINISHED_HALF_CB_ID, dma_half_transfer_handler);
}

/**
 * @brief Update the color of all the LEDs in the strip
 */
void ws2812_update() {
  if (output_state != OUTPUT_IDLE) {
    return;
  }

  // Prefill buffer with LED data
  current_led = 0;
  led_fill_led_pwm_data(0, &pwm_led_data[0]);
  if (led_count > 1) {
    current_led++;
    led_fill_led_pwm_data(1, &pwm_led_data[BITS_PER_LED]);
  }

  // Start output
  output_state = OUTPUT_DATA;
  needs_update = 0;
  start_output(PWM_BUFFER_SIZE);
}

/**
 * @brief Queues up another LED update, if necessary
 * If an LED is set during the current update, this will queue a follow-up
 * update.
 *
 * If an update is not happening, this kicks off a new update.
 */
void should_update(size_t led) {
#if (AUTO_UPDATE == 1)
  if (output_state == OUTPUT_IDLE) {
    ws2812_update();
  } else if (led <= current_led) {
    needs_update = 1;
  }
#endif
}

/**
 * @brief  Start reset pulse sequence
 */
static void led_start_reset_pulse() {
  reset_counter = 0;
  output_state = OUTPUT_RESET;
  pwm_led_data[0] = 0;
  start_output(1);
}

/**
 * @brief  Prepares data from memory for PWM output for timer
 * Memory is in format R,G,B, while PWM must be configured in G,R,B[,W]
 *
 * @param[in]  ledIdx LED index to set the color
 * @param[out] ptr    Output array with at least BITS_PER_LED-words of memory
 */
static void led_fill_led_pwm_data(size_t ledIdx, uint16_t* ptr) {
  // HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
  int8_t i;
  size_t byteIdx = BYTES_PER_LED * ledIdx;

  // Color bytes for this LED
  uint8_t red = led_colors[byteIdx + 0];
  uint8_t green = led_colors[byteIdx + 1];
  uint8_t blue = led_colors[byteIdx + 2];
#if USE_RGBW
  uint8_t white = led_colors[byteIdx + 3];
#endif

  if (ledIdx < led_count) {
    // Cycle through 8 bits of data per color for this LED.
    // Convert each color value into a series of PWM duty cycle bits in G,R,B[,W] order
    // Each loop defines a single bit per color, going from the 8th position to the 1st
    for (i = 7; i >= 0; i--) {
      // Green
      ptr[i] =      (IS_BIT_SET(green, i)) ? binary_1 : binary_0;
      // Red
      ptr[8 + i] =  (IS_BIT_SET(red, i)) ? binary_1 : binary_0;
      // Blue
      ptr[16 + i] = (IS_BIT_SET(blue, i)) ? binary_1 : binary_0;
#if USE_RGBW
      // White
      ptr[24 + i] = (IS_BIT_SET(white, i)) ? binary_1 : binary_0;
#endif /* USE_RGBW */
    }
  }
}

/**
 * Set the LED PWM values to all zeros
 * @param[out] ptr    Output array with at least BITS_PER_LED-words of memory
 */
void led_clear_led_pwm_data(uint16_t* ptr) {
  int8_t i;
  for (i = 0; i < BITS_PER_LED; i++) {
    ptr[i] = 0;
  }
}

/**
 * @brief  Called on each DMA transfer complete or half-transfer complete events and used to prep the next LED values.
 * @param  tc: Transfer complete flag. Set to `1` on TC event, or `0` on HT event
 *
 * @note  This function must be called from DMA interrupt
 *
 * @note  Our buffer contains the LED values for 2 LEDs.
 *        Half transfer complete (HT) will be fired after the first LED has been sent.
 *        Transfer complete (TC) will be fired after the second LED has been sent.
 */
static void led_update_sequence(uint8_t tc) {
  tc = !!tc; // Force value as 1 or 0

  if (output_state == OUTPUT_IDLE) {
    return;
  }

  // Count the reset pulses
  if (output_state == OUTPUT_RESET) {
    reset_counter++;

    if (reset_counter >= RESET_PULSES) {
      stop_output();

      current_led = 0;
      output_state = OUTPUT_IDLE;

      // Start a new update cycle
      if (needs_update) {
        ws2812_update();
      }
    }

    return;
  }
  current_led++;

  // Prepare LED data
  if (current_led < led_count) {
    if (tc) {
      led_fill_led_pwm_data(current_led, &pwm_led_data[BITS_PER_LED]);
    } else {
      led_fill_led_pwm_data(current_led, &pwm_led_data[0]);
    }

  // If this is the last LED, zero out the next buffer
  // because it will take a couple cycles to stop it at the end.
  } else if (current_led == led_count) {
    if (tc) {
      led_clear_led_pwm_data(&pwm_led_data[BITS_PER_LED]);
    } else {
      led_clear_led_pwm_data(&pwm_led_data[0]);
    }

  // All done
  } else {
    stop_output();
    current_led = 0;

    // Send ending reset pulse
    led_start_reset_pulse();
  }
}

/**
 * The DMA data half transfer complete interrupt handler
 */
static void dma_half_transfer_handler(TIM_HandleTypeDef *htim) {
  // HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6);
  led_update_sequence(0);
}

/**
 * The DMA data transfer complete interrupt handler
 */
static void dma_transfer_complete_handler(TIM_HandleTypeDef *htim) {
  // HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8);
  led_update_sequence(1);
}

/**
 * @brief Handles the DMA interrupts
 */
void ws2812b_interrupt_handler(void) {
  HAL_DMA_IRQHandler(&hdma_tim);
}