#ifndef FONT_H
#define FONT_H

#include <stdint.h>
#include "gfx.h"

//render 'ch' at given 'x, y' in color 'draw_color'
//if 'apply_aa' is false antialiasing will be skipped
void draw_char(char ch, int x, int y, color draw_color, size font_size, bool apply_aa);

//convenience function to draw_char on every character in 'str', starting at 
//origin 'x, y'
//automatically handles line wrapping
void draw_string(const char* str, int x, int y, color col, size font_size);

//convenience function to register animation with given parameters
//if 'finished_callback' is provided, that function will be executed upon animation completion
void draw_char_animated(char ch, point origin, size sz, color from, color to, float duration, void* finished_callback, bool apply_aa);

//convenience function for using draw_char_animated on strings
//automatically handles newlines and line wrapping
void draw_string_animated(const char* str, point origin, size font_size, color from, color to, float duration, void* finished_callback, bool apply_aa);

#endif
