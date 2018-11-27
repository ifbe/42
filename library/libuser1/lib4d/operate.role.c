#include "libuser.h"
int str2arg(u8* buf, int len, u8* tmp, int cnt, u8** argv, int max);




void role(u8* buf, int len)
{
	int j,k;
	u8* argv[8];
	u8 tmp[0x1000];

	//if(buf[len-1] <= 0xa)len--;
	//say("%.*s\n", len, buf);

	k = str2arg(buf, len, tmp, 0x1000, argv, 8);
	if(k < 2)return;

}