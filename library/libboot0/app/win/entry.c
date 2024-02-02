#include <stdio.h>
#include <stdarg.h>
#include <conio.h>
#include <direct.h>
#include <windows.h>
#include "libboot.h"




int main(int argc, char** argv)
{
/*
	for(j=1;j<argc;j++){
		k = arg2utf8(argv[j], tmp);
	}
	if(argc <= 1)prep();
*/
	//init world, store args
	void* all = origin_alloc_fromarg(_win32_, main, argc, (u8**)argv);
	origin_create(all, 0, argc, (u8**)argv);
	//call subcmd, until return
	void* thr = bootup_alloc_fromtype(_subcmd_);
	bootup_create(thr, 0, argc, (u8**)argv);

	//main loop

	bootup_delete(thr);
	origin_delete(all);

	return 0;
}
