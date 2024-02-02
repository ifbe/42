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

	//init world, store args
	void* all = origin_alloc_fromarg(_start_, main, 0 ,0);
	origin_create(all, 0, 0, 0);
	//call subcmd, until return
	void* thr = bootup_alloc_fromtype(_kernel_);
	bootup_create(thr, 0, 0, 0);

	bootup_delete(thr);
	origin_delete(all);
	return 0;
}
