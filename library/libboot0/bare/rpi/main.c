#include "libboot.h"




int main(u32 r0, u32 r1, u32 r2)
{
	void* all = origincreate(_start_, main, 0, 0);
	void* wrk = workercreate(_kernel_, 0, 0, 0);
	workerdelete(wrk);
	origindelete(all);
	return 0;
}
