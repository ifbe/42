#include "libsoft.h"
#define _src_ hex32('s','r','c',0)
#define _dst_ hex32('d','s','t',0)




int fv2str_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	float f[10];
	say("@fv2str_read\n");

	relationread((void*)(self->chip), _src_, 0, 0, f, 10);
	return 0;
}
int fv2str_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	int j,cnt;
	float* f;
	u8 tmp[256];
	say("@fv2str_write:%d\n", len);

	cnt = 0;
	f = buf;
	for(j=0;j<len;j++){
		if(j == len-1)cnt += mysnprintf(tmp+cnt, 256-cnt, "%f\n", f[j]);
		else cnt += mysnprintf(tmp+cnt, 256-cnt, "%f, ", f[j]);
	}
	return relationwrite((void*)(self->chip), _dst_, 0, 0, tmp, cnt);
}
int fv2str_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int fv2str_start(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int fv2str_create(struct artery* ele, u8* url)
{
	say("@fv2str_create\n");
	return 1;
}
