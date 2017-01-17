#include "pit.h"
#include "isr.h"
#include "vga.h"

uint32_t tick = 0;

static void pit_callback(registers_t regs) {
	tick++;
	term_puts("Tick\n");
}

void pit_init(uint32_t freq) {
	int_add_handler(IRQ0, &pit_callback);

	uint32_t divisor = 1193180 / freq;

	outb(0x43, 0x36);

	uint8_t l = (uint8_t)(divisor & 0xFF);
	uint8_t h = (uint8_t)((divisor >> 8) & 0xFF);

	outb(0x40, l);
	outb(0x40, h);
}
