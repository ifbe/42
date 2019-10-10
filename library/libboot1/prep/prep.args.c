#include "libboot.h"
int role_default();
int role_fromfile(void*, int);




int args_create(int argc, char** argv)
{
	int j;
	if(argc <= 1){
		role_default();
		return 0;
	}
	for(j=0;j<argc;j++){
		role_fromfile(argv[j], 0);
	}
	return 0;
}