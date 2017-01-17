#include "animator.h"
#include "math.h"
#include "font.h"

#define MAX_ANIMATIONS 512

static int current_anim_count = 0;
static char_fade_anim animations[MAX_ANIMATIONS] = {0};

static void finalize_animation(char_fade_anim anim, int idx, bool graceful) {
	//find index of anim in animations array
	//remove this animation from array
	while (idx < current_anim_count) {
		//shift each element back
		animations[idx] = animations[idx + 1];
		idx++;
	}
	current_anim_count--;

	//call finished handler if this is a graceful exit
	if (anim.finished_callback && graceful) {
		anim.finished_callback();
	}
}

void add_animation(char ch, point origin, size font_size, color from, color to, float duration, void* finished_callback, bool apply_aa) {
	//search through animations and see if we're about to overlap an existing one
	for (int i = 0; i < current_anim_count; i++) {
		char_fade_anim check = animations[i];
		if (check.origin.x == origin.x && check.origin.y == origin.y) {
			finalize_animation(check, i, false);
		}
	}

	if (current_anim_count >= MAX_ANIMATIONS) return;

	//set up parameters
	char_fade_anim new;
	new.ch = ch;
	new.origin = origin;
	new.font_size = font_size;
	new.from = from;
	new.current = from;
	new.to = to;
	new.duration = duration;
	new.used_draw_time = 0.0;
	new.finished = false;
	new.finished_callback = finished_callback;
	new.apply_aa = apply_aa;

	//insert into animations
	animations[current_anim_count++] = new;
}

//go through every active animation and increment its step by delta_time
void process_animations(float delta_time) {
	for (int i = 0; i < current_anim_count; i++) {
		char_fade_anim anim = animations[i];
		if (anim.finished) continue;

		color draw_color;
		float completion_percent = anim.used_draw_time / anim.duration;
		for (int i = 0; i < SCREEN_BPP; i++) {
			draw_color.val[i] = (int)lerp(anim.from.val[i], anim.to.val[i], completion_percent);
		}
		anim.used_draw_time += delta_time;

		//heuristic to see if animation is over
		if ((draw_color.val[0] == anim.to.val[0] &&
			draw_color.val[1] == anim.to.val[1] &&
			draw_color.val[2] == anim.to.val[2]) ||
			anim.used_draw_time >= anim.duration) {
			anim.finished = true;
			draw_color = anim.to;

			finalize_animation(anim, i, true);
		}

		draw_char(anim.ch, anim.origin.x, anim.origin.y, draw_color, anim.font_size, anim.apply_aa);
		anim.current = draw_color;

		animations[i] = anim;
	}
}

