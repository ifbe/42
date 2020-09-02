#include "libboot.h"




int main(u32 dtb)
{
	void* all = origincreate(_start_, main, 0, (void*)(u64)dtb);
	void* wrk = workercreate(_kernel_, 0, 0, 0);
	workerdelete(wrk);
	origindelete(all);
	return 0;
}
