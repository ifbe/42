#include "libsoft.h"
#define _src_ hex32('s','r','c',0)
#define _dst_ hex32('d','s','t',0)
int parsefv(float* fbuf, int flen, u8* sbuf, int slen);




int line2fv_read(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	float f[10];
	say("@line2fv_read\n");

	relationread((void*)(self->chip), _src_, f, 10);
	return 0;
}
int line2fv_write(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	int cnt;
	float tmp[9];
	struct element* ele;
	say("@line2fv_write:%d\n", len);
	//printmemory(buf, len);

	ele = (void*)(self->chip);
	if(0 == ele)return 0;


	cnt = parsefv(tmp, 9, buf, len);
	//say("fvcnt: %d\n", cnt);
	if((cnt != 3) && (cnt != 6) && (cnt != 9) ) {
		say("wrong fvcnt: %d\n", cnt);
		return 0;
	}

#define _dst_ hex32('d','s','t',0)
	return relationwrite((void*)ele, _dst_, tmp, cnt);
	return 0;
}
int line2fv_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int line2fv_start(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int line2fv_create(struct element* ele, u8* url)
{
	say("@line2fv_create\n");
	ele->buf = memorycreate(0x1000, 0);
	ele->len = 0;
	return 1;
}
