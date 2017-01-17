#include "memcpy.h"
#include <stdint.h>

void *memcpy(void *restrict s1, const void *restrict s2, size_t n) {
    if(!n) {
		return s1;
	}
    
	char *p1 = (char*)s1;
    const char *p2 = (const char*)s2;

    // calculate the distance to the nearest natural boundary
    size_t offset = (sizeof(size_t) - ((size_t)p1 % sizeof(size_t))) % sizeof(size_t);

    size_t unused;
    // see if it's even worth aligning
    if(n <= sizeof(size_t)) {
        asm volatile("rep movsb;":"=c"(unused):"D"(p1), "S"(p2), "c"(n));
        return s1;
    }

    n -= offset;

    // align p1 on a natural boundary
    asm volatile("rep movsb;":"=D"(p1), "=S"(p2), "=c"(unused):"D"(p1), "S"(p2), "c"(offset));

    // move in size_t size'd blocks
    asm volatile("rep movsl;":"=D"(p1), "=S"(p2), "=c"(unused):"D"(p1), "S"(p2), "c"(n >> 2));

    // clean up the remaining bytes
    asm volatile("rep movsb;":"=c"(unused):"D"(p1), "S"(p2), "c"(n % sizeof(size_t)));

    return s1;
}

void* memset(void* bufptr, int value, size_t size) {
	//how many 32b chunks we can write
	uint32_t num_dwords = size / 4;
	//leftover bytes that couldn't be set in a larger chunk
	uint32_t num_bytes = size % 4;
	//write in 32b chunks
	uint32_t* dest32 = (uint32_t*)bufptr;
	uint8_t* dest8 = ((uint8_t*)bufptr) + (num_dwords * 4);

	uint32_t val32 = value | (value << 8) | (value << 16) | (value << 24);
	uint8_t val8 = (uint8_t)value;

	//write 4 byte chunks
	for (uint32_t i = 0; i < num_dwords; i++) {
		dest32[i] = val32;
	}
	//write leftover bytes
	for (uint32_t i = 0; i < num_bytes; i++) {
		dest8[i] = val8;
	}
	return bufptr;
}

