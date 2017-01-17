#ifndef MATH_H
#define MATH_H

#include <stdint.h>

//linear interpolation between two values at percent
inline float lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

//PRNG
void srand(uint32_t x);
uint32_t rand(void);

#endif
