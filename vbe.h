#ifndef VBE_H
#define VBE_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define VBE_DISPI_IOPORT_INDEX			0x01CE
#define VBE_DISPI_IOPORT_DATA			0x01CF
#define VBE_DISPI_INDEX_ID				0
#define VBE_DISPI_INDEX_XRES			1
#define VBE_DISPI_INDEX_YRES			2
#define VBE_DISPI_INDEX_BPP				3
#define VBE_DISPI_INDEX_BANK			5
#define VBE_DISPI_INDEX_VIRT_WIDTH		6
#define VBE_DISPI_INDEX_VIRT_HEIGHT		7
#define VBE_DISPI_INDEX_X_OFFSET		8
#define VBE_DISPI_INDEX_Y_OFFSET		9

#define VBE_DISPI_BPP_4					0x04
#define VBE_DISPI_BPP_8					0x08
#define VBE_DISPI_BPP_15				0x0F
#define VBE_DISPI_BPP_16				0x10
#define VBE_DISPI_BPP_24				0x18
#define VBE_DISPI_BPP_32				0x20

#define VBE_DISPI_DISABLED				0x00 
#define VBE_DISPI_ENABLED				0x01
#define VBE_DISPI_INDEX_ENABLE			0x04
#define VBE_DISPI_LFB_ENABLED			0x40
#define VBE_DISPI_NOCLEARMEM			0x80

//read/write to VBE registers
void vbe_write_reg(unsigned short idx, unsigned short val);
unsigned short vbe_read_reg(unsigned short idx);

//is VBE available in our environment?
bool vbe_available(void);

//switch video memory bank to 'bank_num'
void vbe_set_bank(unsigned short bank_num);

//perform VBE graphics mode switch using given parameters
//if 'use_lfb' is set pixels can be plotted directly onto a framebuffer
//if 'clear_vmem' is set we'll request VBE clear framebuffer before we recieve it
void vbe_set_video_mode(unsigned int width, unsigned int height, unsigned int depth, bool use_lfb, bool clear_vmem);

#endif

