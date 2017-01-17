[GLOBAL gdt_load] 	; allow this to be called from C
gdt_load:
	mov eax, [esp+4]	; get pointer to GDT, passed as parameter
	lgdt [eax]		; load new GDT pointer

	mov ax, 0x10		; 0x10 is offset in the GDT to data segment
	mov ds, ax		; load all data segment selectors
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	jmp 0x08:.flush		; 0x08 is the offset to our code segment: far jump!
.flush:
	ret

[GLOBAL idt_load] 	; allow this to be called from C
idt_load:
	mov eax, [esp+4] 	; get the pointer to the IDT, passed as a parameter
	lidt [eax] 		; load IDT pointer
	ret

