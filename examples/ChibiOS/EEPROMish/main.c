/**
 * Lights up the next LED in sequence each time you press the button.
 * When you turn the board off and on again, the last LED will be remembered and be
 * the starting LED in the sequence.
 */
#include "ch.h"
#include "hal.h"
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stm32f4xx.h>

// Save the LED index to the first byte address in the EEPROM
#define LED_SAVE_ADDRESS  0

// Number of LEDs
#define LED_NUM           4

// Define sector 1 for non-volatile memory
// This needs to be reflected in the linker file too (STM32F411CEUx_FLASH_NVM.ld)
#define MEM_FLASH_SECTOR  1          // Flash sector to use for non-volatile memory
#define MEM_START_ADDR    0x8004000  // Memory address where non-volatile memory starts.
#define MEM_SIZE          512        // bytes reserved for memory, this can be up to 16k

uint8_t memory_read(uint32_t byte_address);
bool memory_write(uint32_t byte_address, uint8_t value);
void lightLED(uint8_t index);

int leds[] = {
  LINE_LED1,
  LINE_LED2,
  LINE_LED3,
  LINE_LED4,
};

int main(void) {
  // System initializations.
  halInit();
  chSysInit();

  // Set LEDs to digital output
  for (uint8_t i = 0; i < LED_NUM; i++) {
    palSetLineMode(leds[i], PAL_MODE_OUTPUT_PUSHPULL);
  }

  // Read the LED index from memory and light it up
  uint8_t ledIndex = memory_read(LED_SAVE_ADDRESS);
  lightLED(ledIndex);

  uint8_t buttonState = 0;
  uint8_t lastButtonState = 0;
  while (1) {
    buttonState = palReadLine(LINE_BUTTON);

    // Update the LED index when the button is pressed
    if (buttonState && !lastButtonState) {
      ledIndex++;
      if (ledIndex >= LED_NUM) {
        ledIndex = 0;
      }

      // Light up the new LED
      lightLED(ledIndex);

      // Save LED index to memory
      memory_write(LED_SAVE_ADDRESS, ledIndex);
    }

    lastButtonState = buttonState;
    chThdSleepMilliseconds(100);
  }
}

/**
 * Read a byte value from memory
 */
uint8_t memory_read(uint32_t address){
  return *(uint8_t*)(MEM_START_ADDR + address);
}

/**
 * Write a byte value to memory.
 */
bool memory_write(uint32_t address, uint8_t value) {
  // Copy memory to buffer
  uint8_t buffer[MEM_SIZE];
  memcpy(buffer, (uint8_t*)MEM_START_ADDR, MEM_SIZE);

  // Set byte value in buffer
  buffer[address] = value;

  // Erase flash sector
  HAL_FLASH_Unlock();
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGSERR );
  FLASH_Erase_Sector(MEM_FLASH_SECTOR, VOLTAGE_RANGE_3);

  // Write buffer to sector
  uint32_t i;
  uint32_t addr = (uint32_t)MEM_START_ADDR;
  for (i = 0; i < MEM_SIZE; i++, addr++) {
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, addr, buffer[i]) != HAL_OK) {
      HAL_FLASH_Lock();
      return false;
    }
  }

  // Finish up
  HAL_FLASH_Lock();
  return true;
}

/**
 * Light up the LED by index
 */
void lightLED(uint8_t index) {
  // Reset LEDs
  for (uint8_t i = 0; i < LED_NUM; i++) {
    palClearLine(leds[i]);
  }

  // Light the selected LED
  palSetLine(leds[index]);
}
