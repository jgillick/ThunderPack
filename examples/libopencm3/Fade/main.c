/*
  Fade LED 1 (PA0) on and off continuously.
*/
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>

#define MAX_PWM   0xFFF
#define PWM_INC   9

// Setup GPIO pins
static void gpioa_setup(void) {
    // LED1 (PA0) as an alternate function (TIM2_CH1)
    rcc_periph_clock_enable(RCC_GPIOA);
    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO0);
    gpio_set_af(GPIOA, GPIO_AF2, GPIO0);
}

// Setup the timer for PWM
static void timer_setup(void) {
  // Enable Timer2 and reeset to defaults
  rcc_periph_clock_enable(RCC_TIM2);
  rcc_periph_reset_pulse(RST_TIM2);

  // Timer mode:
  //  - No divider
  //  - Alignment edge
  //  - Direction up
  timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
  timer_set_prescaler(TIM2, 0);
  
  // Run the timer continously
  timer_continuous_mode(TIM2);

  // The PWM period is the total clock cycles for both HIGH & LOW signals of the PWM
  // period = HIGH time + LOW time
  timer_set_period(TIM2, MAX_PWM);

  // Enable preload register on channel 1, to allow independent PWM values 
  timer_enable_oc_preload(TIM2, TIM_OC1);

  // PWM is fine in slow compare mode
  timer_set_oc_slow_mode(TIM2, TIM_OC1);

  // Set the timer output to PWM mode
  // The output is active when the counter is less than the compare value (timer_set_oc_value).
  timer_set_oc_mode(TIM2, TIM_OC1, TIM_OCM_PWM1);
  
  // When the timer is active, set the pin output to HIGH
  timer_set_oc_polarity_high(TIM2, TIM_OC1);

  // Enable channel 1 output and enable the counter
  timer_enable_oc_output(TIM2, TIM_OC1);
  timer_enable_counter(TIM2);
}

int main(void) {
  gpioa_setup();
  timer_setup();

  int i;
  uint32_t fadeVal = 0;
  int8_t direction = 1;
  while (1) {
    // Set the LED PWM value
    timer_set_oc_value(TIM2, TIM_OC1, fadeVal);

    // Increment and reverse if we've reached the limits
    fadeVal += PWM_INC * direction;
    if (fadeVal <= 0 || fadeVal >= MAX_PWM) {
      direction *= -1;
    }

    // A brief pause
    for (i = 0; i < 500; i++) __asm__("nop");
  }

  return 0;
}
