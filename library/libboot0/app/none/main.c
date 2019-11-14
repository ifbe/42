#include <stdio.h>
#include <stdlib.h>
#include "libboot.h"




int main(int argc, char** argv)
{
	void* all;
	void* thr;

	all = pwrclkcreate(_main_, 0, 0, 0);
	thr = workercreate(0, 0, argc, argv);

	workerdelete(thr);
	pwrclkdelete(all);
	return 0;
}