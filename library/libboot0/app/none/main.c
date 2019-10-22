#include <stdio.h>
#include <stdlib.h>
#include "libboot.h"




int main(int argc, char** argv)
{
	void* all;
	void* thr;

	all = pwrclkcreate(_main_, 0, argc, argv);
	thr = workercreate(_poller_, 0, 0, 0);

	workerdelete(thr);
	pwrclkdelete(all);
	return 0;
}