#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "vga.h"
#include "vbe.h"
#include "gfx.h"
#include "pit.h"
#include "desc_tables.h"

void __kern_bootstrap(void) {
	term_init();

	gdt_init();
	idt_init();
	//pit_init(50);

	term_puts("Testing VGA driver. Testing newline\nThis text should be on a new line. Is this text wrapping to next line?");

	vbe_set_video_mode(SCREEN_WIDTH, SCREEN_HEIGHT, 0x18, true, true);
	perform_text_render_demo();

	for (;;) {
		asm("hlt");
	}
}
