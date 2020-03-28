#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/flash.h>
#include <libopencm3/cm3/systick.h>

#include "thunderpack.h"

static volatile uint64_t _millis = 0;
void sys_tick_handler(void);

// Setup systic timer & interrupt handler
static void _systick_setup(void) {
  systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
  STK_CVR = 0; // start at 0

  // In order to trigger an interrupt every millisecond, we can set the reload
  // value to be the speed of the processor / 1000 - 1
  systick_set_reload(rcc_ahb_frequency / 1000 - 1);

  // Enable interrupts & timer
  systick_interrupt_enable();
  systick_counter_enable();
}

// Initialize the thunderpack clock & systick handler
void thunderpack_clock_init(void){
  // Set chip to 96Mhz
  struct rcc_clock_scale rcc_hse_24mhz_3v3 = {
    .pllm = 24,
    .plln = 384,
    .pllp = 4,
    .pllq = 8,
    .pllr = 0,
    .pll_source = RCC_CFGR_PLLSRC_HSE_CLK,
    .hpre = RCC_CFGR_HPRE_DIV_NONE,
    .ppre1 = RCC_CFGR_PPRE_DIV_2,
    .ppre2 = RCC_CFGR_PPRE_DIV_NONE,
    .voltage_scale = PWR_SCALE1,
    .flash_config = FLASH_ACR_DCEN | FLASH_ACR_ICEN | FLASH_ACR_LATENCY_2WS,
    .ahb_frequency  = 96000000,
    .apb1_frequency = 48000000,
    .apb2_frequency = 96000000,
  };
	rcc_clock_setup_pll(&rcc_hse_24mhz_3v3);

  // Setup systic timer & interrupt handler
  _systick_setup();
}

// Get the current value of the millis counter
uint64_t millis(void) {
    return _millis;
}


void delay(uint64_t duration) {
    const uint64_t until = millis() + duration;
    while (millis() < until);
}

// Systick interrupt handler.
void sys_tick_handler(void) {
    // Increment our monotonic clock
    _millis++;
}