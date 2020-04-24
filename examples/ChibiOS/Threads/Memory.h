
#ifndef MEMORY_H
#define MEMORY_H

uint8_t memory_read(uint32_t byte_address);
bool memory_write(uint32_t byte_address, uint8_t value);

#endif