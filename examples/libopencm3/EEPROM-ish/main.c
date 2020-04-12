/**
 * Light up the next LED in sequence each time you press the button.
 * When you turn the board off and on again, the last LED will be remembered and be
 * the starting LED in the sequence.
 */
#include <string.h> 
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/flash.h>
#include "thunderpack.h"

// Save the LED index to the first byte address in the EEPROM
#define LED_SAVE_ADDRESS  0

// Define sector 1 for non-volatile memory
// This needs to be reflected in the linker file too (STM32F411CEUx_FLASH_NVM.ld)
#define MEM_FLASH_SECTOR  1          // Flash sector to use for non-volatile memory
#define MEM_START_ADDR    0x8004000  // Memory address where non-volatile memory starts.
#define MEM_SIZE          512        // bytes reserved for memory, this can be up to 16k

uint8_t memory_read(uint32_t address);
void memory_write(uint32_t address, uint8_t value);
static void lightLED(uint8_t index);
static void gpioSetup(void);

int main(void) {
  gpioSetup();

  // Read the LED index from EEPROM and light it up
  uint8_t ledIndex = memory_read(LED_SAVE_ADDRESS);
  lightLED(ledIndex);

  // Change the lit LED when the button is pressed
  uint8_t buttonState = 0;
  uint8_t lastButtonState = 0;
  while (1) {
    buttonState = gpio_get(GPIOB, GPIO4);

    // Update the LED index when the button changes state
    if (buttonState && !lastButtonState) {
      ledIndex++;
      if (ledIndex > 3) {
        ledIndex = 0;
      }

      // Light up the new LED
      lightLED(ledIndex);

      // Save LED index to EEPROM
      memory_write(LED_SAVE_ADDRESS, ledIndex);
    }

    lastButtonState = buttonState;
  }

  return 0;
}

/**
 * Read a byte value from memory
 */
uint8_t memory_read(uint32_t address) {
  return *(uint8_t*)(MEM_START_ADDR + address);
}

/**
 * Write a byte value to memory.
 */
void memory_write(uint32_t address, uint8_t value) {
  // Copy memory to buffer
  uint8_t buffer[MEM_SIZE];
  memcpy(buffer, (uint8_t*)MEM_START_ADDR, MEM_SIZE);

  // Set byte value in buffer
  buffer[address] = value;

  // Erase flash sector
  flash_unlock();
  flash_clear_status_flags();
  flash_erase_sector(MEM_FLASH_SECTOR, FLASH_CR_PROGRAM_X8);
  flash_wait_for_last_operation();
  
  // Write buffer to sector
  uint32_t i;
  uint32_t addr = (uint32_t)MEM_START_ADDR;
  for (i = 0; i < MEM_SIZE; i++, addr++) {
    flash_program_byte(addr, buffer[i]);
  }
  
  // Finish up
  flash_lock();
}

/**
 * Light up the LED by index
 */
static void lightLED(uint8_t index) {
  thunderpack_clock_init();

  // Reset LEDs
  gpio_clear(GPIOA, GPIO0);
  gpio_clear(GPIOA, GPIO1);
  gpio_clear(GPIOA, GPIO2);
  gpio_clear(GPIOA, GPIO3);

  // Light the selected LED
  switch(index) {
    case 3:
      gpio_set(GPIOA, GPIO3);
      break;
    case 2:
      gpio_set(GPIOA, GPIO2);
      break;
    case 1:
      gpio_set(GPIOA, GPIO1);
      break;
    default:
      gpio_set(GPIOA, GPIO0);
  }
}

static void gpioSetup(void) {
  // set LED1 - LED4 (PA0 - PA03) to output
  rcc_periph_clock_enable(RCC_GPIOA);
  gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO0);
  gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO1);
  gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO2);
  gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO3);

  // Button (PB4) to input, open drain
  rcc_periph_clock_enable(RCC_GPIOB);
  gpio_mode_setup(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO4);
}
