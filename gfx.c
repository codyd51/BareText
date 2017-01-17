#include "gfx.h"
#include "memcpy.h"

static uint8_t double_buf[SCREEN_HEIGHT * SCREEN_WIDTH * SCREEN_BPP] = {0};

int gfx_bpp() {
	return SCREEN_BPP;
}

void transfer_screen_data() {
	//copy double buffer data to real video memory
	uint8_t* raw_vmem = VBE_DISPI_LFB_PHYSICAL_ADDRESS;
	uint8_t* raw_double_buf = double_buf;

	//video memory uses bank switching
	//figure out how many banks we'll need to write to
	int banks_needed = sizeof(double_buf) / BANK_SIZE;
	for (int bank = 0; bank <= banks_needed; bank++) {
		vbe_set_bank(bank);
		memcpy(raw_vmem, raw_double_buf + (BANK_SIZE * bank), BANK_SIZE);
	}
}

void putpixel(int x, int y, color c) {
	//TODO bpp should adjust based on graphics mode
	int offset = (y * SCREEN_WIDTH * SCREEN_BPP) + x * SCREEN_BPP;

	//simply write to double buffer for each byte in a pixel
	for (int i = 0; i < SCREEN_BPP; i++) {
		double_buf[offset + i] = c.val[SCREEN_BPP - 1 - i];
	}
}

