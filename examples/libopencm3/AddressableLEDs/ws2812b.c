#include <stdlib.h>
#include <math.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/dma.h>
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
typedef enum output_states {
  IDLE,
  DATA,
  RESET,
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
static volatile output_states_t output_state = IDLE;

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

void        start_output(void);
void        timer_init(void);
void        dma_init(void);
void        stop_output(void);
void        update_leds(void);
void        should_update(size_t led);
void        led_clear_led_pwm_data(uint16_t* ptr);
static void led_fill_led_pwm_data(size_t ledIdx, uint16_t* ptr);
static void led_update_sequence(uint8_t tc);

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
  uint16_t dataLen = 1;
  if (output_state == DATA) {
    dataLen = PWM_BUFFER_SIZE;
  }

#ifdef DMA_STREAM_NUM
  dma_set_number_of_data(DMA, DMA_STREAM_NUM, dataLen);
  dma_enable_stream(DMA, DMA_STREAM_NUM);
#else
  dma_set_number_of_data(DMA, DMA_CHANNEL, dataLen);
  dma_enable_channel(DMA, DMA_CHANNEL);
#endif
}

/**
 * @brief  Stop the reset data DMA & Timer output.
 */
void stop_output() {
#ifdef DMA_STREAM_NUM
  dma_disable_stream(DMA, DMA_STREAM_NUM);
#else
  dma_disable_channel(DMA, DMA_CHANNEL);
#endif
  timer_set_oc_value(TIMER, TIMER_CHANNEL, 0);

#ifdef DMA_IRQ
  nvic_clear_pending_irq(DMA_IRQ);
#endif
}

/**
 * @brief Setup the timer to output 800kHz
 */
void timer_init() {
  uint32_t pwm_period = (rcc_ahb_frequency / SIGNAL_HZ) - 1;

  // GPIO
  rcc_periph_clock_enable(DATA_PORT_RCC);
  gpio_mode_setup(DATA_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, DATA_PIN);
  gpio_set_af(DATA_PORT, DATA_PIN_AF, DATA_PIN);

  // Timer
  rcc_periph_clock_enable(TIMER_RCC);

  timer_set_prescaler(TIMER, 0);
  timer_set_period(TIMER, pwm_period);
  timer_set_mode(TIMER, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
  timer_continuous_mode(TIMER);
  timer_enable_preload(TIMER);

  // PWM output compare
  timer_set_oc_mode(TIMER, TIMER_CHANNEL, TIM_OCM_PWM1);
  timer_enable_oc_preload(TIMER, TIMER_CHANNEL);
  timer_set_oc_value(TIMER, TIMER_CHANNEL, 0);
  timer_set_oc_polarity_high(TIMER, TIMER_CHANNEL);
  timer_set_oc_slow_mode(TIMER, TIMER_CHANNEL);

  // Events & output
  timer_enable_irq(TIMER, TIM_DIER_CC2DE); // Enable the Capture/Compare DMA request event
  timer_enable_oc_output(TIMER, TIMER_CHANNEL); // Enable capture/compare output

  // Enable
  timer_enable_counter(TIMER);

  // Save the binary 1 & 0 PWM values so we don't need to calculate them at runtime
  binary_1 = ceil(pwm_period * BINARY_1_DUTY_MULTIPLIER);
  // binary_0 = ceil(pwm_period * BINARY_1_DUTY_MULTIPLIER);
  binary_0 = floor(pwm_period * BINARY_0_DUTY_MULTIPLIER);
}

/**
 * @brief Setup the DMA to Timer
 */
void dma_init() {
  if (DMA == DMA1) {
    rcc_periph_clock_enable(RCC_DMA1);
  } else if (DMA == DMA2) {
    rcc_periph_clock_enable(RCC_DMA2);
  }

  // Enable Interrupt
#ifdef DMA_IRQ
  nvic_clear_pending_irq(DMA_IRQ);
  nvic_set_priority(DMA_IRQ, 0);
  nvic_enable_irq(DMA_IRQ);
#endif

#ifdef DMA_STREAM_NUM
  dma_stream_reset(DMA, DMA_STREAM_NUM);
  dma_set_dma_flow_control(DMA, DMA_STREAM_NUM);
  dma_set_transfer_mode(DMA, DMA_STREAM_NUM, DMA_SxCR_DIR_MEM_TO_PERIPHERAL);
  dma_disable_peripheral_increment_mode(DMA, DMA_STREAM_NUM);
  dma_enable_memory_increment_mode(DMA, DMA_STREAM_NUM);
  dma_set_memory_size(DMA, DMA_STREAM_NUM, DMA_SxCR_MSIZE_16BIT);
  dma_set_peripheral_size(DMA, DMA_STREAM_NUM, DMA_SxCR_PSIZE_16BIT);
  dma_enable_circular_mode(DMA, DMA_STREAM_NUM);
  dma_set_priority (DMA, DMA_STREAM_NUM, DMA_SxCR_PL_VERY_HIGH);
  dma_channel_select(DMA, DMA_STREAM_NUM, DMA_CHANNEL);

  dma_set_peripheral_address(DMA, DMA_STREAM_NUM, (uint32_t)&TIMER_CCR);
  dma_set_memory_address(DMA, DMA_STREAM_NUM, (uint32_t)&pwm_led_data);

  dma_enable_half_transfer_interrupt(DMA, DMA_STREAM_NUM);
  dma_enable_transfer_complete_interrupt(DMA, DMA_STREAM_NUM);

#else // DMA_REQUEST
  dma_channel_reset(DMA, DMA_CHANNEL);
  dma_set_channel_request(DMA, DMA_CHANNEL, DMA_REQUEST);
  dma_set_read_from_memory(DMA, DMA_CHANNEL);
  dma_disable_peripheral_increment_mode(DMA, DMA_CHANNEL);
  dma_enable_memory_increment_mode(DMA, DMA_CHANNEL);
  dma_set_memory_size(DMA, DMA_CHANNEL, DMA_CCR_MSIZE_16BIT);
  dma_set_peripheral_size(DMA, DMA_CHANNEL, DMA_CCR_PSIZE_16BIT);
  dma_enable_circular_mode(DMA, DMA_CHANNEL);
  dma_set_priority (DMA, DMA_CHANNEL, DMA_CCR_PL_VERY_HIGH);

  dma_set_peripheral_address(DMA, DMA_CHANNEL, (uint32_t)&TIMER_CCR);
  dma_set_memory_address(DMA, DMA_CHANNEL, (uint32_t)&pwm_led_data);

  dma_enable_half_transfer_interrupt(DMA, DMA_CHANNEL);
  dma_enable_transfer_complete_interrupt(DMA, DMA_CHANNEL);
#endif
}

/**
 * @brief  Update the color of all the LEDs in the strip
 */
void update_leds() {
  if (output_state != IDLE) {
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
  output_state = DATA;
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
  if (output_state == IDLE) {
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
  output_state = RESET;
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

  if (output_state == IDLE) {
    return;
  }

  // Count the reset pulses
  if (output_state == RESET) {
    reset_counter++;

    if (reset_counter >= RESET_PULSES) {
      stop_output();

      current_led = 0;
      output_state = IDLE;

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
void ws2812b_interrupt_handler() {
#ifdef DMA_STREAM_NUM
  // Transfer complete
  if (dma_get_interrupt_flag(DMA, DMA_STREAM_NUM, DMA_TCIF) != 0) {
    dma_clear_interrupt_flags(DMA, DMA_STREAM_NUM, DMA_TCIF);
    led_update_sequence(1);
  // Half transfer complete
  } else if (dma_get_interrupt_flag(DMA, DMA_STREAM_NUM, DMA_HTIF) != 0) {
    dma_clear_interrupt_flags(DMA, DMA_STREAM_NUM, DMA_HTIF);
    led_update_sequence(0);
  }
#else
  // Transfer complete
  if (dma_get_interrupt_flag(DMA, DMA_CHANNEL, DMA_TCIF) != 0) {
    dma_clear_interrupt_flags(DMA, DMA_CHANNEL, DMA_TCIF);
    led_update_sequence(1);
  // Half transfer complete
  } else if (dma_get_interrupt_flag(DMA, DMA_CHANNEL, DMA_HTIF) != 0) {
    dma_clear_interrupt_flags(DMA, DMA_CHANNEL, DMA_HTIF);
    led_update_sequence(0);
  }
#endif
}

#ifdef DMA_ISR_FUNC
DMA_ISR_FUNC {
  ws2812b_interrupt_handler();
}
#endif
