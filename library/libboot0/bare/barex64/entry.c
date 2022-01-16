#include "libboot.h"




int start()
{
	void* all = origin_create(_start_, start, 0, 0);
	void* wrk = bootup_create(_kernel_, 0, 0, 0);
	bootup_delete(wrk);
	origin_delete(all);
	return 0;
}
