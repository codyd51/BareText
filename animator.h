#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "gfx.h"
#include <stdbool.h>

typedef struct animation {
	color from;
	color to;
	float duration;
	float used_draw_time;
	char ch;
	point origin;
	size font_size;
	color current;
	bool finished;
	void(*finished_callback)(void);
	bool apply_aa;
} char_fade_anim;

//spawn animation of character 'ch' at 'origin' with font size 'font_size',
//interpolating from 'from' to 'to' over 'duration' seconds
//if 'finish_callback' is provided, a handler will be dispatched when the animation finished
//if 'apply_aa' is false, antialiasing code will be skipped
void add_animation(char ch, point origin, size font_size, color from, color to, float duration, void* finished_callback, bool apply_aa);

//update every running animation with 'delta_time' seconds since last update
void process_animations(float delta_time);

#endif
