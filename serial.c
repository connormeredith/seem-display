#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "lib/rs232.h"
#include "serial.h"
#include "memory.h"

#define BAUD B115200
#define PORT /dev/cu.usbmodemfa131

static unsigned char serialBuffer[0x2000];
static unsigned char frameBuffer[0x1AFF];
static uint16_t frameBufferIndex = 0;

const static char mode[]={ '8', 'N', '1', 0 };

void serial_init(void) {
	printf("Waiting for serial port...\n");
	if(RS232_OpenComport(0, BAUD, mode)) {
		printf("Can not open comport\n");

		exit(1);
	}
	usleep(1000000);
	RS232_cputs(0, "1");
}

int serial_get_frame_buffer(void) {
	uint16_t n = RS232_PollComport(0, serialBuffer, 0x2000);
	uint16_t i = 0;

	if(n > 0) {
		for(i = 0; i < n; i++) {
			frameBuffer[frameBufferIndex] = serialBuffer[i];
			frameBufferIndex++;

			if(frameBufferIndex == 0x1B00) break;
		}

		if(frameBufferIndex == 0x1B00) {
			frameBufferIndex = 0;

			for(frameBufferIndex = 0; frameBufferIndex < 0x1B00; frameBufferIndex++) {
				printf("BYTE::0x%x\n", frameBuffer[frameBufferIndex]);
				memory_write(frameBufferIndex, frameBuffer[frameBufferIndex]);
			}

			frameBufferIndex = 0;

			if(i < n) {
				for(i; i < n; i++) {
					frameBuffer[frameBufferIndex] = serialBuffer[i];
					frameBufferIndex++;
				}
			}

			return 1;
		}
	}

	return 0;
}