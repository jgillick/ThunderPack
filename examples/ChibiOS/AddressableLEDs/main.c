/**
 * Changes the LED strip from red to blue to green, by replacing each LED
 * sequentially down the LED strip.
 *
 * Set LED_COUNT to the number of LEDs which are in your strip.
 */
#include <math.h>
#include "ch.h"
#include "hal.h"

#include "ws2812b_conf.h"
#include "ws2812b.h"

#define LED_COUNT 16
#define TIM_CHANNEL_INTERNAL TIMER_CHANNEL - 1

/**
 * @brief Wipe a color across the entire LED string.
 */
void colorWipe(uint8_t r, uint8_t g, uint8_t b) {
  size_t i;
  for (i = 0; i < LED_COUNT; i++) {
    ws2812b_led(i, r, g, b);
    chThdSleepMilliseconds(50);
  }
}

// static PWMConfig pwmcfg;
// uint16_t binary_1;
// uint16_t binary_0;
// static const stm32_dma_stream_t *dma_stream;
// static uint16_t pwm_led_data[2];
// static void dma_interrupt_handler(PWMDriver *pwm, uint32_t flags);
/*
int main2(void) {
  halInit();
  chSysInit();

  // DMA
  uint32_t stream_id = STM32_DMA_STREAM_ID(1, 5);
  dma_stream = dmaStreamAlloc(stream_id, 0, (stm32_dmaisr_t)dma_interrupt_handler, &PWM_DRIVER);
  dmaStreamSetPeripheral(dma_stream, &(PWM_DRIVER.tim->CCR[TIM_CHANNEL_INTERNAL]));
  dmaStreamSetMemory0(dma_stream, pwm_led_data);
  dmaStreamSetMode(dma_stream,
    STM32_DMA_CR_CHSEL(DMA_CHANNEL)
    | STM32_DMA_CR_DIR_M2P     // Memory to peripheral
    | STM32_DMA_CR_PSIZE_HWORD // Peripheral data: half-word
    | STM32_DMA_CR_MSIZE_HWORD // Memory data: half-word
    | STM32_DMA_CR_MINC        // Memory increment
    | STM32_DMA_CR_CIRC        // Circular mode
    | STM32_DMA_CR_HTIE        // Half transfer interrupt enabled
    | STM32_DMA_CR_TCIE        // Transfer complete interrupt enabled
    | STM32_DMA_CR_PL(3)       // Priority: very high
  );

  // Timer
  pwmcfg = (PWMConfig){
    .frequency  = STM32_SYSCLK,
    .period     = (STM32_SYSCLK / SIGNAL_HZ) - 1,
    .callback   = NULL,
    .channels   = {
      [0 ... 3] = { .mode = PWM_OUTPUT_DISABLED, .callback = NULL },
    },
    .cr2        = TIM_DIER_CC2IE,
    // .dier       = TIM_DIER_UDE,
    .dier       = TIM_DIER_CC2DE,
  };
  pwmcfg.channels[TIM_CHANNEL_INTERNAL].mode = PWM_OUTPUT_ACTIVE_HIGH;
  palSetPadMode(DATA_PORT, DATA_PIN, PAL_MODE_ALTERNATE(DATA_PIN_AF_NUM));
  pwmStart(&PWM_DRIVER, &pwmcfg);
  // pwmEnableChannel(&PWM_DRIVER, TIM_CHANNEL_INTERNAL, PWM_PERCENTAGE_TO_WIDTH(&PWMD3, 5000));
  pwmEnableChannel(&PWM_DRIVER, TIM_CHANNEL_INTERNAL, 0);

  binary_1 = ceil(pwmcfg.period * BINARY_1_DUTY_MULTIPLIER);
  binary_0 = floor(pwmcfg.period * BINARY_0_DUTY_MULTIPLIER);

  pwm_led_data[0] = binary_1;
  pwm_led_data[1] = binary_1;

  // Output
  dmaStreamSetTransactionSize(dma_stream, 2);
  dma_stream->stream->CR |= STM32_DMA_CR_TCIE | STM32_DMA_CR_HTIE;
  dmaStreamEnable(dma_stream);

  uint32_t str_addr = (uint32_t)&dma_stream;
  binary_0 = binary_1;

  while (1) {}

  return 0;
}
*/
/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void) {
  // Initialize the system
  halInit();
  chSysInit();

  // Initialize the ws2812b lib
  ws2812b_init(LED_COUNT);

  // Start making colors
  while (1) {
    colorWipe(0xFF, 0x00, 0x00); // Red
    colorWipe(0x00, 0xFF, 0x00); // Green
    colorWipe(0x00, 0x00, 0xFF); // Blue
  }

  return 0;
}

// static void dma_interrupt_handler(PWMDriver *pwm, uint32_t flags) {
// }