#include "libboot.h"




int start()
{
	void* all = origincreate(_start_, start, 0, 0);
	void* wrk = workercreate(_kernel_, 0, 0, 0);
	workerdelete(wrk);
	origindelete(all);
	return 0;
}