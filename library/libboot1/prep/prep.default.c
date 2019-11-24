#include "libuser.h"
int role_fromfile(void*, int);
int str2arg(u8* buf, int len, u8* tmp, int cnt, u8** argv, int max);



void role_default()
{
	supplycreate(_std_, 0, 0, 0);
}
void role(u8* buf, int len)
{
	int j,argc;
	u8* argv[8];
	u8 tmp[256];

	//if(buf[len-1] <= 0xa)len--;
	//say("%.*s\n", len, buf);

	if('=' == buf[4]){
		argc = 2;
		argv[0] = buf;
		argv[1] = &buf[5];
	}
	else{
		argc = str2arg(buf, len, tmp, 256, argv, 8);
		if(argc <= 1)return;
	}
	role_fromfile(argv[1], 0);
}
