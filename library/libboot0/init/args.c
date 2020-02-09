#include "libboot.h"
#define _wnd_ hex32('w','n','d', 0)
int role_default();
int role_fromfile(void*, int);




int args_create(int argc, char** argv)
{
	int j;
	if(0 == argv){
		supplycreate(_wnd_, 0, 0, 0);
		workercreate(_poller_, 0, 0, 0);
	}

	if(argc <= 1){
		role_default();
		return 0;
	}

	for(j=1;j<argc;j++){
		role_fromfile(argv[j], 0);
	}
	return 0;
}