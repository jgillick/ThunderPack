#include <stdlib.h>
#include <math.h>
#include "stm32l0xx_hal.h"
#include "stm32l0xx_hal_dma.h"
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
 * @brief Return if a bit value is set.
 */
#define IS_BIT_SET(VAL, BIT_IDX)  (VAL & (1 << BIT_IDX))

/**
 * Number of bits in the PWM buffer used by the DMA controller
 */
#define PWM_BUFFER_SIZE  (2 * BITS_PER_LED)

/**
 * Output state values
 */
#define OUTPUT_IDLE         0  // Nothing is being output
#define OUTPUT_DATA         1  // LED data is being sent
#define OUTPUT_RESET        2  // Reset after last color

/**
 * @brief The number of LEDs in the strip
 */
static uint8_t led_count = 1;

/**
 * @brief Array of 3x (or 4x) number of leds (R, G, B[, W] colors)
 */
static uint8_t *led_colors;

/**
 * @brief Array holding the next two LED colors, as an array of PWM values to represent each bit.
 */
static uint16_t pwm_led_data[PWM_BUFFER_SIZE];

/**
 * @brief The current state of the output.
 */
static volatile uint8_t output_state = OUTPUT_IDLE;

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

void        start_output(uint32_t SrcAddress, uint32_t DataLength);
void        timer_init(void);
void        dma_init(void);
void        stop_output(void);
void        update_leds();
void        should_update(size_t led);
static void led_fill_led_pwm_data(size_t ledIdx, uint16_t* ptr);
static void led_update_sequence(uint8_t tc);
static void dma_half_transfer_handler(DMA_HandleTypeDef *hdma);
static void dma_transfer_complete_handler(DMA_HandleTypeDef *hdma);

static void led_start_reset_pulse();
#if USE_RGBW
void        led_set_color_internal(size_t index, uint8_t r, uint8_t g, uint8_t b, uint8_t w);
#else
void        led_set_color_internal(size_t index, uint8_t r, uint8_t g, uint8_t b);
#endif
void        led_set_color_rgb_internal(size_t index, uint32_t rgbw);


/**
 * @brief  Initialize the ws2823b library and peripherals.
 * @param  num_leds  The number of LEDs in the LED strip.
 */
void ws2812b_init(size_t num_leds) {
  // Reserve memory
  led_count = num_leds;
  led_colors = (uint8_t *)malloc(BYTES_PER_LED * led_count);

  // Initialize Peripherals
  dma_init();
  timer_init();

  // Set all LEDs to off
#if USE_RGBW
  led_set_color_all(0, 0, 0, 0);
#else
  led_set_color_all(0, 0, 0);
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
void led_set_color(size_t index, uint8_t r, uint8_t g, uint8_t b
#if USE_RGBW
, uint8_t w) {
  led_set_color_internal(index, r, g, b, w);
#else
) {
  led_set_color_internal(index, r, g, b);
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
void led_set_color_all(uint8_t r, uint8_t g, uint8_t b
#if USE_RGBW
, uint8_t w
#endif /* USE_RGBW */
) {
  size_t index;
  for (index = 0; index < led_count; index++) {
#if USE_RGBW
    led_set_color_internal(index, r, g, b, w);
#else
    led_set_color_internal(index, r, g, b);
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
void led_set_color_rgb(size_t index, uint32_t rgbw) {
  led_set_color_rgb_internal(index, rgbw);
  should_update(index);
}

/**
 * @brief Set the same color on all LEDs with a single 16 or 32-bit value to represent RGB[W]
 * @see   led_set_color_rgb()
 * @param  rgbw   Color value
 */
void led_set_color_all_rgb(uint32_t rgbw) {
  size_t index;
  for (index = 0; index < led_count; index++) {
    led_set_color_rgb_internal(index, rgbw);
  }
  should_update(0);
}

/**
 * @brief Internal version of led_set_color which does not automatically call should_update
 */
void led_set_color_internal(size_t index, uint8_t r, uint8_t g, uint8_t b
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
 * @brief Internal version of led_set_color_rgb which does not automatically call should_update
 */
void led_set_color_rgb_internal(size_t index, uint32_t rgbw) {
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
void start_output(uint32_t SrcAddress, uint32_t DataLength) {
  __HAL_TIM_ENABLE(&htim);
  HAL_DMA_Start_IT(&hdma_tim, SrcAddress, (uint32_t)&htim.Instance->CCR1, DataLength);
}

/**
 * @brief  Stop the reset data DMA & Timer output.
 */
void stop_output() {
  HAL_DMA_Abort_IT(&hdma_tim);
}

/**
 * @brief Setup the timer to output 800kHz
 */
void timer_init() {
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  GPIO_InitTypeDef GPIO_PWMStruct = {0};

  // GPIO Setup
  __HAL_RCC_GPIOA_CLK_ENABLE();
  GPIO_PWMStruct.Pin = DATA_PIN;
  GPIO_PWMStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_PWMStruct.Pull = GPIO_NOPULL;
  GPIO_PWMStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_PWMStruct.Alternate = DATA_PIN_AF;
  HAL_GPIO_Init(DATA_PORT, &GPIO_PWMStruct);

  // Timer
  SET_BIT(RCC->APB1ENR, TIMER_RCC_BIT);

  htim.Instance = TIMER;
  htim.Init.Prescaler = 0;
  htim.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim.Init.Period = (SystemCoreClock / SIGNAL_HZ) - 1;
  htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  HAL_TIM_PWM_Init(&htim);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim, &sMasterConfig);

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_ENABLE;
  HAL_TIM_PWM_ConfigChannel(&htim, &sConfigOC, TIMER_CHANNEL);

  // Enable the update request DMA event
  // (it will update the PWM value faster then capture/compare)
  SET_BIT(TIMER->DIER, TIM_DIER_UDE);

  // Disable capture/compare DMA event
  CLEAR_BIT(TIMER->DIER, TIM_DIER_CC1DE);

  // Enable capture/compare output
  SET_BIT(TIMER->CCER, TIM_CCER_CC1E);

  // Enable the timer
  __HAL_TIM_ENABLE(&htim);

  // Save the binary 1 & 0 PWM values so we don't need to calculate them at runtime
  binary_1 = ceil(htim.Init.Period * BINARY_1_DUTY_MULTIPLIER);
  binary_0 = floor(htim.Init.Period * BINARY_0_DUTY_MULTIPLIER);
}

/**
 * @brief Setup the DMA to Timer
 */
void dma_init() {
  __HAL_RCC_DMA1_CLK_ENABLE();
  hdma_tim.Instance = DMA_CHANNEL;
  hdma_tim.Init.Request = DMA_REQUEST;
  hdma_tim.Init.Direction = DMA_MEMORY_TO_PERIPH;
  hdma_tim.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_tim.Init.MemInc = DMA_MINC_ENABLE;
  hdma_tim.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
  hdma_tim.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
  hdma_tim.Init.Mode = DMA_CIRCULAR;
  hdma_tim.Init.Priority = DMA_PRIORITY_VERY_HIGH;

  hdma_tim.XferCpltCallback = dma_transfer_complete_handler;
  hdma_tim.XferHalfCpltCallback = dma_half_transfer_handler;

  HAL_DMA_Init(&hdma_tim);
  __HAL_LINKDMA(&htim, hdma[DMA_HANDLE_INDEX], hdma_tim);
}

/**
 * @brief  Update the color of all the LEDs in the strip
 */
void update_leds() {
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
  start_output((uint32_t)pwm_led_data, PWM_BUFFER_SIZE);
}

/**
 * @brief Queues up another LED update, if necessary
 * If an LED is set during the current update, this will queue a follow-up
 * update.
 *
 * If an update is not happening, this kicks off a new update.
 */
void should_update(size_t led) {
  if (output_state == OUTPUT_IDLE) {
    update_leds();
  } else if (led <= current_led) {
    needs_update = 1;
  }
}

/**
 * @brief  Start reset pulse sequence
 */
static void led_start_reset_pulse() {
  reset_counter = 0;
  output_state = OUTPUT_RESET;
  pwm_led_data[0] = 0;
  start_output((uint32_t)pwm_led_data, 1);
}

/**
 * @brief  Prepares data from memory for PWM output for timer
 * Memory is in format R,G,B, while PWM must be configured in G,R,B[,W]
 *
 * @param[in]  ledIdx LED index to set the color
 * @param[out] ptr    Output array with at least BITS_PER_LED-words of memory
 */
static void led_fill_led_pwm_data(size_t ledIdx, uint16_t* ptr) {
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
        update_leds();
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
static void dma_half_transfer_handler(DMA_HandleTypeDef *hdma) {
  led_update_sequence(0);
}

/**
 * The DMA data transfer complete interrupt handler
 */
static void dma_transfer_complete_handler(DMA_HandleTypeDef *hdma) {
  led_update_sequence(1);
}

/**
 * @brief Handles the DMA interrupts
 */
void ws2812b_interrupt_handler(void) {
  HAL_DMA_IRQHandler(&hdma_tim);
}