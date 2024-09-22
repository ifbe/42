#include "libboot.h"




int start()
{
	//init world, store args
	void* all = origin_alloc_fromarg(_start_, start, 0 ,0);
	origin_create(all, 0, 0, 0);
	//call subcmd, until return
	void* thr = bootup_alloc_fromtype(_kernel_);
	bootup_create(thr, 0, 0, 0);

	bootup_delete(thr);
	origin_delete(all);
	return 0;
}
