/**
 * Lights up the next LED in sequence each time you press the button.
 * When you turn the board off and on again, the last LED will be remembered and be
 * the starting LED in the sequence.
 */
#include <string.h> 
#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include "thunderpack.h"

// Save the LED index to the first byte address in the EEPROM
#define LED_SAVE_ADDRESS  0

// Number of LEDs
#define LED_NUM           4

// Define sector 0 for non-volatile memory
// This needs to be reflected in the linker file too (STM32F411CEUx_FLASH.ld)
#define MEM_FLASH_SECTOR  1          // Flash sector to use for non-volatile memory
#define MEM_START_ADDR    0x8004000  // Memory address where non-volatile memory starts.
#define MEM_SIZE          512        // bytes reserved for memory, this can be up to 16k

uint8_t memory_read(uint32_t byte_address);
bool memory_write(uint32_t byte_address, uint8_t value);
static void gpio_init(void);
void light_led(uint8_t index);

int led_pins[] = {
  GPIO_PIN_0, // LED 1
  GPIO_PIN_1, // LED 2
  GPIO_PIN_2, // LED 3
  GPIO_PIN_3, // LED 4
};

int main(void) {
  // Init the system
  HAL_Init();
  thunderpack_clock_init();

  // Setup GPIO
  gpio_init();
  
  // Read the LED index from memory and light it up
  uint8_t ledIndex = memory_read(LED_SAVE_ADDRESS);
  light_led(ledIndex);

  // Change the lit LED when the button is pressed
  uint8_t buttonState = 0;
  uint8_t lastButtonState = 0;
  while (1) {
    buttonState = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4);

    // Update the LED index when the button changes state
    if (buttonState && !lastButtonState) {
      ledIndex++;
      if (ledIndex > 3) {
        ledIndex = 0;
      }

      // Light up the new LED
      light_led(ledIndex);

      // Save LED index to memory
      memory_write(LED_SAVE_ADDRESS, ledIndex);
    }

    lastButtonState = buttonState;
    HAL_Delay(10); 
  }
}

/**
 * Light up the LED by index
 */
void light_led(uint8_t index) {
  for (uint8_t i = 0; i < LED_NUM; i++) {
    HAL_GPIO_WritePin(GPIOA, led_pins[i], GPIO_PIN_RESET);  
  }
  HAL_GPIO_WritePin(GPIOA, led_pins[index], GPIO_PIN_SET);
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
  * GPIO Initialization Function
  */
static void gpio_init(void) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  // LEDs
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GPIO_PIN_RESET);
  GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // Button - PA4
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}