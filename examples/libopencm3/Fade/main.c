/*
  Fade LED 1 (PA0) on and off continuously.
*/
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
// #include <libopencm3/cm3/nvic.h>
// #include <libopencm3/stm32/exti.h>
// #include <libopencmsis/core_cm3.h>

#define MAX_PWM   0xFFFF // We're using a 16-bit timer, might as well use all of it
#define PWM_INC   45

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
  
  //timer_set_repetition_counter(TIM2, 0);

  // Disable auto-reload buffering, so the PWM value is changed as soon as it's set.
  // Otherwise, the value will be updated at the next cycle.
  // timer_disable_preload(TIM2);

  // Run the timer continously
  timer_continuous_mode(TIM2);

  // The PWM period is the total clock cycles for both HIGH & LOW signals of the PWM
  // period = HIGH time + LOW time
  timer_set_period(TIM2, MAX_PWM);

  // Disable all timer outputs
  // timer_disable_oc_output(TIM2, TIM_OC1);
  // timer_disable_oc_output(TIM2, TIM_OC2);
  // timer_disable_oc_output(TIM2, TIM_OC3);
  // timer_disable_oc_output(TIM2, TIM_OC4);

  // https://hasanyavuz.ozderya.net/?p=437
  // timer_set_deadtime(TIM2, 0);

  // timer_set_enabled_off_state_in_idle_mode(TIM2);

  // timer_disable_break(TIM2);
  // timer_set_break_polarity_high(TIM2);
  // timer_disable_break_automatic_output(TIM2);
  // timer_set_break_lock(TIM2, TIM_BDTR_LOCK_OFF);

  /* -- OC1 configuration -- */
  /* Configure global mode of line 1. */
  // timer_disable_oc_clear(TIM2, TIM_OC1);

  // Enable preload register on channel 1, to allow independent PWM values 
  timer_enable_oc_preload(TIM2, TIM_OC1);

  // PWM is fine in slow compare mode
  timer_set_oc_slow_mode(TIM2, TIM_OC1);

  // Set the timer output to PWM mode
  // The output is active when the counter is less than the compare value (timer_set_oc_value).
  timer_set_oc_mode(TIM2, TIM_OC1, TIM_OCM_PWM1);
  
  // When the timer is active, set the pin output to HIGH
  timer_set_oc_polarity_high(TIM2, TIM_OC1);
  // timer_set_oc_idle_state_set(TIM2, TIM_OC1);

  // 50% PWM
  // timer_set_oc_value(TIM2, TIM_OC1, MAX_PWM/2);

  // Enable channel 1 output
  timer_enable_oc_output(TIM2, TIM_OC1);
  // timer_enable_preload(TIM2);
  // timer_enable_break_main_output(TIM2);
  
  // Now that it's setup, enable the counter
  timer_enable_counter(TIM2);
}

int main(void) {
  gpioa_setup();
  timer_setup();

  int i;
  uint16_t fadeVal = 0;
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
