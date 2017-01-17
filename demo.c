#include "demo.h"
#include "font.h"
#include "animator.h"

color rand_color() {
	color ret;
	ret.val[0] = rand() % 255;
	ret.val[1] = rand() % 255;
	ret.val[2] = rand() % 255;
	return ret;
}

static char* sample_string = "The quick brown fox jumps over the lazy dog. 4x SSAA text rendering in full ";
static size small_font = {8, 8};
static size normal_font = {16, 16};
static size big_font = {32, 32};

void spawn_rgb_label() {
	int chars_on_line = SCREEN_WIDTH / normal_font.width;
	int sample_string_y_lines = (strlen(sample_string) / chars_on_line) + 2;
	int sample_string_x_offset = (strlen(sample_string) % chars_on_line) + 1;
	float anim_dur = 2.0;

	static color last_to[3] = {0};
	point rgb_orig = {(normal_font.width * sample_string_x_offset), (normal_font.height * sample_string_y_lines)};

	color to = rand_color();
	draw_char_animated('R', rgb_orig, normal_font, last_to[0], to, anim_dur, spawn_rgb_label, true);
	last_to[0] = to;

	to = rand_color();
	rgb_orig.x += normal_font.width;
	draw_char_animated('G', rgb_orig, normal_font, last_to[1], to, anim_dur, spawn_rgb_label, true);
	last_to[1] = to;

	to = rand_color();
	rgb_orig.x += normal_font.width;
	draw_char_animated('B', rgb_orig, normal_font, last_to[2], to, anim_dur, spawn_rgb_label, true);
	last_to[2] = to;
}

void perform_text_render_demo() {
	srand(1484438382);

	color white = {255, 255, 255};
	color black = {0, 0, 0};
	color red = {255, 0, 0};
	color green = {0, 255, 0};
	color blue = {0, 0, 255};

	char* welcome_blurb = "- BareText text renderer/mini OS\n\nFeatures:\n* Font renderer w/ 4x supersample antialiasing\n* Animation subsystem\n* Mini kernel\n* PRNG\n* 1024x768 VBE graphics\n\n\nThis project was created in x86 assembly and C.\n It runs on its own OS..\n It was created for Hack Arizona 2017.\nCompiled, BareText is less than 25kb";

	char* enlarged = "Enlarged to show antialiasing";

	point origin = {0, 0};
	color from = black;
	draw_string_animated("Small Text!", origin, small_font, from, white, 2.0, NULL, true);

	origin.y += normal_font.height * 2;
	from = black;
	draw_string_animated(sample_string, origin, normal_font, from, white, 2.0, NULL, true);
	spawn_rgb_label();

	origin.y += normal_font.height * 3;
	color different = {227, 23, 84};
	draw_string_animated(welcome_blurb, origin, normal_font, from, different, 3.0, NULL, true);

	origin.y += normal_font.height * 17;
	color different2 = {64, 23, 227};
	draw_string_animated(enlarged, origin, big_font, from, different2, 5.0, NULL, true);

	origin.y += normal_font.height * 3;
	draw_string_animated("This text is not antialiased.", origin, big_font, from, different2, 5.0, NULL, false);

	while (1) {
		process_animations(0.1);
		transfer_screen_data();
	}
}

