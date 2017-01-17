#include "vbe.h"

void vbe_write_reg(unsigned short idx, unsigned short val) {
    outw(VBE_DISPI_IOPORT_INDEX, idx);
    outw(VBE_DISPI_IOPORT_DATA, val);
}
 
unsigned short vbe_read_reg(unsigned short idx) {
    outw(VBE_DISPI_IOPORT_INDEX, idx);
    return inw(VBE_DISPI_IOPORT_DATA);
}
 
bool vbe_available(void) {
    //return (vbe_read_reg(VBE_DISPI_INDEX_ID) == VBE_DISPI_ID4);
	return true;
}
 
void vbe_set_video_mode(unsigned int width, unsigned int height, unsigned int depth, bool use_lfb, bool clear_vmem) {
    vbe_write_reg(VBE_DISPI_INDEX_ENABLE, VBE_DISPI_DISABLED);
    vbe_write_reg(VBE_DISPI_INDEX_XRES, width);
    vbe_write_reg(VBE_DISPI_INDEX_YRES, height);
    vbe_write_reg(VBE_DISPI_INDEX_BPP, depth);
    vbe_write_reg(VBE_DISPI_INDEX_ENABLE, VBE_DISPI_ENABLED  |
					(use_lfb	? VBE_DISPI_LFB_ENABLED : 0) |
					(clear_vmem ? 0 : VBE_DISPI_NOCLEARMEM));
}
 
void vbe_set_bank(unsigned short bank_num) {
    vbe_write_reg(VBE_DISPI_INDEX_BANK, bank_num);
}
