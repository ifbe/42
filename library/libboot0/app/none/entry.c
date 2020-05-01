#include <stdio.h>
#include <stdlib.h>
#include "libboot.h"




int main(int argc, char** argv)
{
	void* all = origincreate(_main_, 0, argc, (u8**)argv);
	void* thr = workercreate(0, 0, 0, 0);

	workerdelete(thr);
	origindelete(all);
	return 0;
}
