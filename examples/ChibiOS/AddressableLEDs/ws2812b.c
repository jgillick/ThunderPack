#include <stdlib.h>
#include <math.h>
#include "hal.h"
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

// Chibios channels are zero indexed
#define TIM_CHANNEL_INTERNAL TIMER_CHANNEL - 1

/**
 * Output state values
 */
typedef enum output_states {
  OUTPUT_IDLE,  // Nothing is being output
  OUTPUT_DATA,  // LED data is being sent
  OUTPUT_RESET, // Reset after last color
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
 * @brief Array holding the next two LED colors, as an array of PWM values to represent each bit.
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
 * @brief Timer config object
 */
static PWMConfig pwmcfg;

/**
 * @brief DMA stream object
 */
static const stm32_dma_stream_t *dma_stream;

void        start_output(void);
void        timer_init(void);
void        dma_init(void);
void        stop_output(void);
void        update_leds(void);
void        should_update(size_t led);
static void led_fill_led_pwm_data(size_t ledIdx, uint16_t* ptr);
static void led_update_sequence(uint8_t tc);
static void dma_interrupt_handler(PWMDriver *pwm, uint32_t flags);

static void led_start_reset_pulse(void);
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
  dma_init();
  timer_init();

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
void start_output() {
  // Set data buffer size
  uint32_t dataLen = 1;
  if (output_state == OUTPUT_DATA) {
    dataLen = PWM_BUFFER_SIZE;
  }
  dmaStreamSetTransactionSize(dma_stream, dataLen);

  // The CR register gets cleared out by dmaStreamDisable,
  // but not set back correctly by dmaStreamEnable
#ifdef DMA_STREAM_NUM
  dma_stream->stream->CR |= STM32_DMA_CR_TCIE | STM32_DMA_CR_HTIE;
#else
  dma_stream->channel->CCR |= STM32_DMA_CR_TCIE | STM32_DMA_CR_HTIE;
#endif

  // Enable output
  dmaStreamEnable(dma_stream);
}

/**
 * @brief  Stop the reset data DMA & Timer output.
 */
void stop_output() {
  dmaStreamDisable(dma_stream);
}

/**
 * @brief Setup the timer to output 800kHz
 */
void timer_init() {
  pwmcfg = (PWMConfig){
    .frequency  = STM32_SYSCLK,
    .period     = (STM32_SYSCLK / SIGNAL_HZ) - 1,
    .callback   = NULL,
    .channels   = {
      [0 ... 3] = { .mode = PWM_OUTPUT_DISABLED, .callback = NULL },
    },
    // .cr2        = TIM_CR2_CCDS,
    // .dier       = 0 //TIM_DIER_UDE // TIM_DIER_CC2DE,
    .cr2        = 0,
    .dier       = TIM_DIER_CC2DE,
  };
  pwmcfg.channels[TIM_CHANNEL_INTERNAL].mode = PWM_OUTPUT_ACTIVE_HIGH;

  palSetPadMode(GPIOA, 4, PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(GPIOA, 5, PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(GPIOA, 6, PAL_MODE_OUTPUT_PUSHPULL);

  pwmStart(&PWM_DRIVER, &pwmcfg);
  palSetPadMode(DATA_PORT, DATA_PIN, PAL_MODE_ALTERNATE(DATA_PIN_AF_NUM));
  pwmEnableChannel(&PWM_DRIVER, TIM_CHANNEL_INTERNAL, 0);

  // Save the binary 1 & 0 PWM values so we don't need to calculate them at runtime
  binary_1 = ceil(pwmcfg.period * BINARY_1_DUTY_MULTIPLIER);
  binary_0 = floor(pwmcfg.period * BINARY_0_DUTY_MULTIPLIER);
}

/**
 * @brief Setup the DMA to Timer
 */
void dma_init() {
#ifdef DMA_STREAM_NUM
  uint32_t stream_id = STM32_DMA_STREAM_ID(DMA_NUM, DMA_STREAM_NUM);
#else
  uint32_t stream_id = STM32_DMA_STREAM_ID(DMA_NUM, DMA_CHANNEL);
#endif 

  dma_stream = dmaStreamAlloc(stream_id, 0, (stm32_dmaisr_t)dma_interrupt_handler, &PWM_DRIVER);
  dmaStreamSetPeripheral(dma_stream, &(PWM_DRIVER.tim->CCR[TIM_CHANNEL_INTERNAL]));
  dmaStreamSetMemory0(dma_stream, pwm_led_data);
  dmaStreamSetMode(dma_stream,
#ifdef DMA_REQUEST
    STM32_DMA_CR_CHSEL(DMA_REQUEST)
#else 
    STM32_DMA_CR_CHSEL(DMA_CHANNEL)
#endif
    | STM32_DMA_CR_DIR_M2P     // Memory to peripheral
    | STM32_DMA_CR_PSIZE_HWORD // Peripheral data: half-word
    | STM32_DMA_CR_MSIZE_HWORD // Memory data: half-word
    | STM32_DMA_CR_MINC        // Memory increment
    | STM32_DMA_CR_CIRC        // Circular mode
    | STM32_DMA_CR_HTIE        // Half transfer interrupt enabled
    | STM32_DMA_CR_TCIE        // Transfer complete interrupt enabled
    | STM32_DMA_CR_PL(3)       // Priority: very high
  );
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
  start_output();
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
  palTogglePad(GPIOA, 4);
  pwm_led_data[0] = 0;
  start_output();
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
      palTogglePad(GPIOA, 4);
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
 * @brief Handles the DMA interrupts
 */
static void dma_interrupt_handler(PWMDriver *pwm, uint32_t flags) {
  if (pwm == &PWM_DRIVER) {
    // Transfer complete
    if (flags & STM32_DMA_ISR_TCIF) {
      palTogglePad(GPIOA, 5);
      led_update_sequence(1);
    // Half transfer complete
    } else if (flags & STM32_DMA_ISR_HTIF) {
      led_update_sequence(0);
    }
  }
}