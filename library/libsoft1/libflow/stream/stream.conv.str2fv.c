#include "libsoft.h"
#define _src_ hex32('s','r','c',0)
#define _dst_ hex32('d','s','t',0)
int parsefv(float* fbuf, int flen, u8* sbuf, int slen);




int str2fv_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	float f[10];
	struct element* ele;
	say("@str2fv_read\n");

	ele = (void*)(self->chip);
	if(0 == ele)return 0;

	relationread(ele, _src_, 0, 0, f, 10);
	return 0;
}
int str2fv_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	int cnt;
	float tmp[9];
	struct element* ele;
	say("@str2fv_write:%d\n", len);
	//printmemory(buf, len);

	ele = (void*)(self->chip);
	if(0 == ele)return 0;

	cnt = parsefv(tmp, 9, buf, len);
	return relationwrite(ele, _dst_, 0, 0, tmp, cnt);
}
int str2fv_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int str2fv_start(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int str2fv_create(struct element* ele, u8* url)
{
	say("@str2fv_create\n");
	ele->buf0 = memorycreate(0x1000, 0);
	ele->len = 0;
	return 1;
}
