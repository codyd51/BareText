#include "isr.h"

isr_t interrupt_handlers[256] = {0};

void pic_ack(uint32_t interrupt) {
	if (interrupt < 0x20 || interrupt > (0x28 + 7)) {
		return;
	}

	if (interrupt < 0x28) {
		outb(0xA0, 0x20);
	}
	outb(0x20, 0x20);
}

void isr_handler(registers_t regs) {
	/*
	uint8_t int_no = regs.int_no;
	pic_ack(int_no);
	*/

	int int_no = regs.int_no;
	if (interrupt_handlers[int_no]) {
		isr_t handler = interrupt_handlers[int_no];
		handler(regs);
	}
	else {
		term_puts("Unhandled ISR ");
		term_put_dec(int_no);
		term_putc('\n');
	}
}

void irq_handler(registers_t regs) {
	if (regs.int_no >= 40) {
		outb(0xA0, 0x20);
	}
	outb(0x20, 0x20);

	int int_no = regs.int_no;
	if (interrupt_handlers[regs.int_no] != 0) {
		isr_t handler = interrupt_handlers[int_no];
		handler(regs);
	}
}

void int_add_handler(uint8_t n, isr_t handler) {
	interrupt_handlers[n] = handler;
}
