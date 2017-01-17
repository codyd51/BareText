#include "vga.h"
#include "gfx.h"
#include <stddef.h>

static point cursor_loc = {0, 0};
static size term_size = {80, 25};

static uint8_t raw_color;

//base of BIOS vmem
#define VGA_BASE 0xB8000
static uint16_t* term_buffer = (uint16_t*)VGA_BASE;

static inline uint8_t get_raw_color(vga_color bg, vga_color fg) {
	return fg | bg << 4;
}

static inline uint16_t create_entry(unsigned char c, uint8_t color) {
	return (uint16_t)c | (uint16_t)color << 8;
}

size_t strlen(const char* str) {
	size_t len = 0;
	while (str[len]) {
		len++;
	}
	return len;
}

void term_init() {
	cursor_loc.x = 0;
	cursor_loc.y = 0;
	raw_color = get_raw_color(VGA_COLOR_BLACK, VGA_COLOR_GREEN);

	//TODO implement memset
	for (int y = 0; y < term_size.height; y++) {
		for (int x = 0; x < term_size.width; x++) {
			const size_t idx = y * term_size.width + x;
			term_buffer[idx] = create_entry(' ', raw_color);
		}
	}
}

void term_set_raw_color(uint8_t color) {
	raw_color = color;
}

void term_place_entry(char c, uint8_t color, point p) {
	size_t idx = p.y * term_size.width + p.x;
	term_buffer[idx] = create_entry(c, color);
}

void term_putc(char c) {
	if (c != '\n') {
		term_place_entry(c, raw_color, cursor_loc);
	}

	//go to next line if necessary
	if (cursor_loc.x++ >= term_size.width || c == '\n') {
		cursor_loc.x = 0;
		cursor_loc.y++;
		if (cursor_loc.y >= term_size.height) {
			cursor_loc.y = 0;
		}
	}
}

void term_write(const char* data, int size) {
	for (int i = 0; i < size; i++) {
		term_putc(data[i]);
	}
}

void term_puts(const char* data) {
	term_write(data, strlen(data));
}

void term_put_dec(int num) {
	char buf[32];
	itoa(num, &buf);
	term_puts(buf);
}

