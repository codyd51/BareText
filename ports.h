#ifndef PORTS_H
#define PORTS_H

#include <stdint.h>

//standard in/out port communication helper functions

//output byte
void outb(uint16_t port, uint8_t val);
//output word
void outw(uint16_t port, uint16_t val);
//output long
void outl(uint16_t port, uint32_t val);

//read byte
uint8_t inb(uint16_t port);
//read word
uint16_t inw(uint16_t port);
//read long
uint32_t inl(uint16_t port);

#endif
