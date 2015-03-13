#include <SDL2/SDL.h>
#include <stdint.h>

#include "display.h"
#include "memory.h"

static int pixelColor(uint_least8_t, uint_least8_t);

// ZX Spectrum resolution.
const int WIDTH = 256;
const int HEIGHT = 192;

// Screen vars.
SDL_Window* window;
SDL_Event event;

void display_init(void) {
  SDL_Init(SDL_INIT_VIDEO);
  window = SDL_CreateWindow("SEEM", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
  display_render();
}

void display_service_events(void) {
	while(SDL_PollEvent(&event)) {
    switch(event.type) {
      case SDL_QUIT:
        exit(1);
      default:
        break;
    }
  }
}

void display_render(void) {
	SDL_Surface* surface = SDL_GetWindowSurface(window);
	SDL_LockSurface(surface);

	unsigned int *ptr = (unsigned int*)surface->pixels;

	uint8_t actualRow, displayByte, attributeByte, currentPixelBit;
	int rowPointer;
	uint8_t offset = 0;
	int vCount;

	for(vCount = 0; vCount < 192; vCount++) {
		int hCount;

		if((vCount % 8) == 0 && vCount != 0) offset++;
		if((vCount % 64) == 0 && vCount != 0) offset = (offset - 8) + 64;

		actualRow = (((vCount % 8) * 8) + offset);
		rowPointer = actualRow * (surface->pitch / sizeof(unsigned int));

		for(hCount = 0; hCount < 256; hCount++) {
			displayByte = memory_read(0x4000 + (hCount >> 3) + (vCount << 5));
			attributeByte = memory_read(0x5800 + ((actualRow >> 3) << 5) + (hCount >> 3));
			currentPixelBit = 7 - (hCount % 8);

			ptr[rowPointer + hCount] = pixelColor(attributeByte, (displayByte & (1 << currentPixelBit)));
		}
	}

	SDL_UnlockSurface(surface);
	SDL_UpdateWindowSurface(window);
}

static int pixelColor(uint8_t attributeByte, uint8_t isForeground) {
	// Converts the 3 color bits of an attribute byte into hex depending on their value.
	static int spectrumColor[8] = { 0x00, 0xFF, 0xFF0000, 0xFF00FF, 0xFF00, 0xFFFF, 0xFFFF00, 0xFFFFFF };

	uint8_t colorByte = (isForeground) ? (attributeByte & 0x7) : ((attributeByte & 0x38) >> 3);
	return spectrumColor[colorByte];
}