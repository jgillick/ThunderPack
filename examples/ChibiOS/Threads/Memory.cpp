/**
 * Uses a sector of internal flash memory as a non-volatile memory storage,
 * like EEPROM.
 *
 * Learn more:
 * https://www.st.com/resource/en/application_note/dm00036065-eeprom-emulation-in-stm32f40xstm32f41x-microcontrollers-stmicroelectronics.pdf
 */

#include <string.h>
#include <stdbool.h>
#include <stm32f4xx.h>
#include "./Memory.h"

// Flash memory settings
#define MEM_FLASH_SECTOR  1          // Flash sector to use for non-volatile memory
#define MEM_START_ADDR    0x8004000  // Memory address where non-volatile memory starts.
#define MEM_SIZE          512        // bytes reserved for memory, this can be up to 16k

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