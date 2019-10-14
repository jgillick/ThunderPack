/**
 * Lights up the next LED in sequence each time you press the button.
 * When you turn the board off and on again, the last LED will be remembered and be
 * the starting LED in the sequence.
 */

#include "ch.h"
#include "hal.h"
#include <stm32l0xx.h>

// Use the first byte of EEPROM to store the LED index
#define LED_INDEX_ADDR 0

// Convert a relative EEPROM address to the absolute memory address
#define EEPROM_ADDR(addr) (DATA_EEPROM_BASE + addr)

/**
 * Write a value from EEPROM.
 * The address param is relative to the starting memory position 
 * of the EEPROM memory sector. So `0` is the first byte of EEPROM.
 */
uint32_t read_eeprom(uint32_t address) {
  return *(uint8_t*) EEPROM_ADDR(address);
}

/**
 * Write a value to EEPROM.
 * The address param is relative to the starting memory position 
 * of the EEPROM memory sector. So `0` is the first byte of EEPROM.
 */
void write_eeprom(uint32_t address, uint32_t data) {
  uint32_t eepromAddr = EEPROM_ADDR(address);
  HAL_FLASHEx_DATAEEPROM_Unlock();
  HAL_FLASHEx_DATAEEPROM_Erase(eepromAddr);
  HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAM_WORD, eepromAddr, data);
  HAL_FLASHEx_DATAEEPROM_Lock();
}


/**
 * Light up the LED by index
 */
void lightLED(uint8_t index) {
  // Reset LEDs
  palClearLine(LINE_LED_1);
  palClearLine(LINE_LED_2);
  palClearLine(LINE_LED_3);
  palClearLine(LINE_LED_4);

  // Light the selected LED
  switch(index) {
    case 3:
      palSetLine(LINE_LED_4);
      break;
    case 2:
      palSetLine(LINE_LED_3);
      break;
    case 1:
      palSetLine(LINE_LED_2);
      break;
    default:
      palSetLine(LINE_LED_1);
  }
}

int main(void) {
  // System initializations.
  halInit();
  chSysInit();

  // Set LEDs to digital output
  palSetLineMode(LINE_LED_1, PAL_MODE_OUTPUT_PUSHPULL);
  palSetLineMode(LINE_LED_2, PAL_MODE_OUTPUT_PUSHPULL);
  palSetLineMode(LINE_LED_3, PAL_MODE_OUTPUT_PUSHPULL);
  palSetLineMode(LINE_LED_4, PAL_MODE_OUTPUT_PUSHPULL);

  // Read the LED index from EEPROM and light it up
  uint8_t ledIndex = read_eeprom(LED_INDEX_ADDR);
  lightLED(ledIndex);

  uint8_t buttonState = 0;
  uint8_t lastButtonState = 0;
  while (1) {
    buttonState = palReadLine(LINE_USER_BTN);

    // Update the LED index when the button WAS pressed, but is no longer.
    if (!buttonState && lastButtonState) {
      ledIndex++;
      if (ledIndex > 3) {
        ledIndex = 0;
      }

      // Light up the new LED
      lightLED(ledIndex);

      // Save LED index to EEPROM
      write_eeprom(LED_INDEX_ADDR, ledIndex);
    }

    lastButtonState = buttonState;
    chThdSleepMilliseconds(10);
  }
}
