#include "font8x8.h"
#include "gfx.h"
#include <stdint.h>
#include "math.h"
#include "animator.h"

#define CHAR_HEIGHT 8
#define CHAR_WIDTH 8
#define SSAA_FACTOR 3

//generate supersampled bitmap of font character
static void generate_supersampled_map(uint32_t* supersample, char ch) {
	//for every col in SS bitmap
	for (int y = 0; y < CHAR_HEIGHT * SSAA_FACTOR; y++) {
		uint32_t ssaa_row = 0;

		//get row from sampled bitmap
		int font_row = font8x8_basic[(int)ch][y / SSAA_FACTOR];

		//for every row in SS bitmap
		for (int x = 0; x < CHAR_WIDTH * SSAA_FACTOR; x++) {
			//copy state of sampled bitmap
			if ((font_row >> (x / SSAA_FACTOR)) & 1) {
				ssaa_row |= (1 << x);
			}
		}
		supersample[y] = ssaa_row;
	}
}

void draw_char(char ch, int x, int y, color draw_color, size font_size, bool apply_aa) {
	//quit early if char is whitespace
	if (ch == ' ' || ch == '\t') return;

	point p = {x, y};

	//find scale factor of default font size to size requested
	int scale_x = font_size.width / CHAR_WIDTH;
	int scale_y = font_size.height / CHAR_HEIGHT;

	uint32_t supersample[CHAR_HEIGHT * SSAA_FACTOR] = {0};
	generate_supersampled_map(&supersample, ch);

	uint32_t idx = ((y * SCREEN_WIDTH * gfx_bpp()) + (x * gfx_bpp()));

	uint8_t* font_bitmap = font8x8_basic[(int)ch];

	for (int draw_y = 0; draw_y < font_size.height; draw_y++) {
		//get the corresponding y of default font size
		int font_y = draw_y / scale_y;
		int font_row = font8x8_basic[(int)ch][font_y];

		if (font_y >= CHAR_HEIGHT) continue;

		if (!apply_aa) {
			for (int draw_x = 0; draw_x < font_size.width; draw_x++) {
				int font_x = draw_x / scale_x;
				if ((font_row >> font_x) & 1) {
					putpixel(p.x + draw_x, p.y + draw_y, draw_color);
				}
			}
			continue;
		}

		for (int draw_x = 0; draw_x < font_size.width; draw_x++) {
			//corresponding x of default font size
			int font_x = draw_x / scale_x;

			//antialiasing
			//holds number of 'on' pixels in supersampled region
			//'on' pixels / total pixel count in SSAA region = alpha of pixel to draw
			int on_count = 0;
			int total_count = 0;

			//go around all supersampled pixels adjacent to current and
			//(square of 9 pixels)
			//record any 'on' pixels in supersample
			for (int dx = -1; dx <= 1; dx++) {
				for (int dy = -1; dy <= 1; dy++) {
					int ssaa_x = (font_x * SSAA_FACTOR) + dx;
					int ssaa_y = (font_y * SSAA_FACTOR) + dy;

					uint32_t ssaa_row = supersample[ssaa_y];
					total_count++;
					if ((ssaa_row >> ssaa_x) & 1) {
						on_count++;
					}
				}
			}

			//'on' pixels / total pixel count in SSAA region = alpha of pixel to draw
			float alpha = (float)on_count / (float)total_count;
			color avg_color = draw_color;
			//set avg_color to color * alpha
			//this is a lerp of background color to text color, at alpha
			color bg_color = {0, 0, 0};

			//linear interpolation of background to text color at 'alpha' level
			for (int i = 0; i < gfx_bpp(); i++) {
				avg_color.val[i] = lerp(bg_color.val[i], avg_color.val[i], alpha);
			}
			//draw this pixel
			putpixel(p.x + draw_x, p.y + draw_y, avg_color);
		}
	}
}

void draw_string(const char* str, int x, int y, color col, size font_size) {
	for (int i = 0; i < strlen(str); i++) {
		//do we need to go to the next line?
		if ((x + font_size.width) >= SCREEN_WIDTH) {
			x = 0;
			//have we run out of screen space?
			if ((y += font_size.height) >= SCREEN_HEIGHT) {
				y = 0;
			}
		}
		draw_char(str[i], x, y, col, font_size, true);
		//increment x for next character
		x += font_size.width;
	}
}

void draw_char_animated(char ch, point origin, size sz, color from, color to, float duration, void* finished_callback, bool apply_aa) {
	add_animation(ch, origin, sz, from, to, duration, finished_callback, apply_aa);
}

void draw_string_animated(const char* str, point origin, size font_size, color from, color to, float duration, void* finished_callback, bool apply_aa) {
	int x = origin.x;
	int y = origin.y;
	for (int i = 0; i < strlen(str); i++) {
		if ((origin.x + font_size.width) >= SCREEN_WIDTH || str[i] == '\n') {
			origin.x = 0;
			if ((origin.y += font_size.height) >= SCREEN_HEIGHT) {
				origin.y = 0;
			}
		}
		draw_char_animated(str[i], origin, font_size, from, to, duration, finished_callback, apply_aa);
		origin.x += font_size.width;
	}
}

