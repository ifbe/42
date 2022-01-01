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
	void* all = origincreate(_win32_, main, argc, (u8**)argv);
	//call mython, until return
	void* thr = bootupcreate(_subcmd_, 0, argc, (u8**)argv);

	bootupdelete(thr);
	origindelete(all);

	return 0;
}
