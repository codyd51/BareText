#ifndef GFX_H
#define GFX_H

#include "vbe.h"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define SCREEN_BPP 3

typedef struct point {
	int x;
	int y;
} point;
typedef struct size {
	int width;
	int height;
} size;

typedef struct color {
	uint8_t val[3];
} color;


#define BANK_SIZE 0x10000
#define VBE_DISPI_LFB_PHYSICAL_ADDRESS 0xA0000

//bytes per pixel of current graphics mode
int gfx_bpp();
//write double buffer data to real video memory
void transfer_screen_data();

#endif
