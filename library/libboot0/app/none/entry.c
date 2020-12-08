#include <stdio.h>
#include <stdlib.h>
#include "libboot.h"




int main(int argc, char** argv)
{
	//init world, store args
	void* all = origincreate(_main_, main, argc, (u8**)argv);
	//call mython, until return
	void* thr = bootupcreate(_myml_, 0, argc, (u8**)argv);

	bootupdelete(thr);
	origindelete(all);
	return 0;
}
