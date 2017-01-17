#ifndef MEMCPY_H
#define MEMCPY_H

#include <stddef.h>

//standard memcpy
//copy 'n' bytes from 's2' to 's1'
//this memcpy implementation utilizes vector registers for increased performance
void *memcpy(void *restrict s1, const void *restrict s2, size_t n);

//standard memset
//sets 'size' bytes starting at 'buf' to 'value'
void* memset(void *buf, int value, size_t size);


#endif
