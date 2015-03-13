#include <stdint.h>
#include <stdio.h>

#include "memory.h"

// Video memory.
uint8_t videoMemory[0x1B00] = { };

uint8_t memory_read(uint16_t address) {
	return videoMemory[(address - 0x4000)];
}

void memory_write(uint16_t address, uint8_t byte) {
	videoMemory[address] = byte;
}