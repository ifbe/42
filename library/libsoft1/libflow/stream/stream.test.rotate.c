#include "libsoft.h"




int rotate_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int rotate_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	int x,y;
	u8 yy,uu,vv;
	struct artery* ele = self->pchip;
	u8* dst = ele->buf0;
	u8* src = buf;
	u8* d8;
	u8* s8;

	for(y=0;y<480;y++){
		for(x=0;x<640;x+=2){
			d8 = dst + (    y)*640*2 + (    x)*2;
			s8 = src + (479-y)*640*2 + (638-x)*2;
			d8[0] = s8[2];
			d8[1] = s8[1];
			d8[2] = s8[0];
			d8[3] = s8[3];
		}
	}
	relationwrite(ele, _dst_, 0, 0, dst, 640*480*2);
	return 0;
}
int rotate_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int rotate_start(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int rotate_delete(struct artery* ele)
{
	return 0;
}
int rotate_create(struct artery* ele, u8* url)
{
	say("@rotate_create\n");
	ele->buf0 = memorycreate(0x100000, 0);
	return 1;
}
