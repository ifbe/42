#include <stdio.h>
#include <stdlib.h>
#include "libboot.h"




int main(int argc, char** argv)
{
	void* all = pwrclkcreate(_main_, 0, argc, (u8**)argv);
	void* thr = workercreate(0, 0, 0, 0);

	workerdelete(thr);
	pwrclkdelete(all);
	return 0;
}
