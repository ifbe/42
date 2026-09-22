#include "libboot.h"
void setentryandstack(void* entry, void* stack);
void setdtb(void*);
//
void example_rpi();




/* must do on pi4
1.use hdmi0, not hdmi1

2.add these to config.txt
#mini uart
enable_uart=1
core_freq=250
#1920x1080
hdmi_mode:1=16
hdmi_group:1=1
hdmi_mode:0=16
hdmi_group:0=1
*/
int main(u32 dtb)
{
	u8 tmp;
	setentryandstack(main, &tmp);
	setdtb((void*)(u64)dtb);

	example_rpi();

	return 0;
}
