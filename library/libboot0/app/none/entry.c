#include <stdio.h>
#include <stdlib.h>
#include "libboot.h"




int main(int argc, char** argv)
{
	//int size = 0x1000000);
	//void* addr = malloc(size);
	//lib42_birth(addr, size);

	//init world, store args
	void* all = origin_alloc_fromarg(_main_, main, argc, (u8**)argv);
	origin_create(all, 0, argc, (u8**)argv);
	//call subcmd, until return
	void* thr = bootup_alloc_fromtype(_subcmd_);
	bootup_create(thr, 0, argc, (u8**)argv);

	//mainloop

	bootup_delete(thr);
	origin_delete(all);

	//lib42_death();
	//free(addr);
	return 0;
}
