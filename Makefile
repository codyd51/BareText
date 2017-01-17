CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra

all:
	#i686-elf-as boot.s -o boot.o
	i686-elf-gcc -c kernel.c -o kernel.o $(CFLAGS)
	i686-elf-gcc -c vga.c -o vga.o $(CFLAGS)
	i686-elf-gcc -c ports.c -o ports.o $(CFLAGS)
	i686-elf-gcc -c vbe.c -o vbe.o $(CFLAGS)
	i686-elf-gcc -c font.c -o font.o $(CFLAGS)
	i686-elf-gcc -c gfx.c -o gfx.o $(CFLAGS)
	i686-elf-gcc -c memcpy.c -o memcpy.o $(CFLAGS)
	i686-elf-gcc -c demo.c -o demo.o $(CFLAGS)
	i686-elf-gcc -c animator.c -o animator.o $(CFLAGS)
	i686-elf-gcc -c math.c -o math.o $(CFLAGS)
	i686-elf-gcc -c desc_tables.c -o desc_tables.o $(CFLAGS)
	i686-elf-gcc -c isr.c -o isr.o $(CFLAGS)
	i686-elf-gcc -c pit.c -o pit.o $(CFLAGS)
	i686-elf-gcc -c std.c -o std.o $(CFLAGS)

	nasm -f elf -o boot.o boot.s 
	nasm -f elf -o desc_table_util.o desc_table_util.s 
	nasm -f elf -o interrupt.o interrupt.s 

	i686-elf-gcc -T linker.ld -o baretext.bin -ffreestanding -O2 -nostdlib boot.o kernel.o vga.o ports.o vbe.o font.o gfx.o memcpy.o demo.o animator.o math.o desc_tables.o desc_table_util.o interrupt.o isr.o pit.o std.o -lgcc

	cp baretext.bin isodir/boot/baretext.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	
run:
	grub-mkrescue -o baretext.iso isodir
	qemu-system-i386 -cdrom baretext.iso

