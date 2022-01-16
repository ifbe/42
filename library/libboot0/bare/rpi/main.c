#include "libboot.h"
void* setdtb();




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
	setdtb((void*)(u64)dtb);

	void* all = origin_create(_start_, main, 0, 0);
	void* wrk = bootup_create(_kernel_, 0, 0, 0);
	bootup_delete(wrk);
	origin_delete(all);
	return 0;
}
