#include <stdio.h>

#include "display.h"
#include "serial.h"
#include "memory.h"

int main(void) {
	display_init();
	serial_init();

	uint16_t memoryPointer = 0x0000;
	uint8_t byte = 0x00;

	for(;;) {
		display_service_events();

		if(serial_get_frame_buffer()) {
			printf("RENDER\n");
			display_render();
		}

		// byte = serial_receive();
		// memory_write(memoryPointer, byte);

		// if(memoryPointer == 0x1AFF) {
		// 	printf("RENDER\n");
		// 	memoryPointer = 0x0000;
		// 	display_render();
		// } else {
		// 	memoryPointer++;
		// }
	}
}