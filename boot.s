MBOOT_PAGE_ALIGN	equ 1<<0	; page-align modules
MBOOT_MEM_INFO		equ 1<<1	; memory map
MBOOT_HEADER_MAGIC	equ 0x1BADB002	; multiboot magic 
MBOOT_HEADER_FLAGS	equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHECKSUM		equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

[BITS 32]

[GLOBAL mboot]
[EXTERN code]			; start of .text section
[EXTERN bss]			; start of .bss
[EXTERN end]			; end of last loadable section

[SECTION .mboot]
mboot:
	dd MBOOT_HEADER_MAGIC	; multiboot magic
	dd MBOOT_HEADER_FLAGS	; grub flags 
	dd MBOOT_CHECKSUM	; ensure above values are correct

	dd mboot		; location of this descriptor
	dd code			; start of .text (code) section
	dd bss			; start of .data section
	dd end			; end of kernel
	dd start		; kernel entry point (initial EIP)

[SECTION .text]
[GLOBAL start]			; entry point
[EXTERN __kern_bootstrap]		; C entry point
start:
	; load initial stack
	mov esp, stack_space
	push esp
	; grub header pointer
	push ebx

	; execute kernel
	call __kern_bootstrap

	; kernel ended, spinloop
	cli
	hlt
	jmp $

; set size of the _start symbol to the current location '.' minus its start
; .size _start, . -_start

[SECTION .bss]
RESB 8192 ; reserve 8kb stack
stack_space:
