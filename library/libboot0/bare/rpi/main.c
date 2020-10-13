#include "libboot.h"
void* setdtb();




int main(u32 dtb)
{
	setdtb((void*)(u64)dtb);

	void* all = origincreate(_start_, main, 0, 0);
	void* wrk = workercreate(_kernel_, 0, 0, 0);
	workerdelete(wrk);
	origindelete(all);
	return 0;
}
