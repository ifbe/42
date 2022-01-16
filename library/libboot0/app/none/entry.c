#include <stdio.h>
#include <stdlib.h>
#include "libboot.h"




int main(int argc, char** argv)
{
	//init world, store args
	void* all = origin_create(_main_, main, argc, (u8**)argv);
	//call mython, until return
	void* thr = bootup_create(_subcmd_, 0, argc, (u8**)argv);

	bootup_delete(thr);
	origin_delete(all);
	return 0;
}
