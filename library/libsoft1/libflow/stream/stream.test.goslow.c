#include "libsoft.h"




int goslow_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	float f[10];
	struct element* ele;
	say("@goslow_read\n");

	ele = self->pchip;
	if(0 == ele)return 0;

	relationread(ele, _src_, 0, 0, f, 10);
	return 0;
}
int goslow_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	struct element* ele;
	say("@goslow_write\n");

	ele = self->pchip;
	if(0 == ele)return 0;

	if(0 == ele->stage1)relationwrite(ele, _src_, 0, 0, buf, len);
	ele->stage1 = (ele->stage1 + 1) % 60;
	return 0;
}
int goslow_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int goslow_start(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int goslow_create(struct element* ele, u8* url)
{
	say("@goslow_create\n");
	ele->stage1 = 0;
	return 1;
}
