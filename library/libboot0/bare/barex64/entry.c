#include "libboot.h"




int start()
{
	void* all = origincreate(_start_, start, 0, 0);
	void* wrk = bootupcreate(_kernel_, 0, 0, 0);
	bootupdelete(wrk);
	origindelete(all);
	return 0;
}
