#include "libsoft.h"




int goslow_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	float f[10];
	struct artery* ele;
	say("@goslow_read\n");

	ele = self->pchip;
	if(0 == ele)return 0;

	relationread(ele, _src_, 0, 0, f, 10);
	return 0;
}
int goslow_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	int j;
	struct artery* ele = self->pchip;
	if(0 == ele)return 0;

	j = ele->stage1;
	say("@goslow_write:%d\n", j);

	if(0 == j){
		say("@goslow.sending\n");
		relationwrite(ele, _dst_, 0, 0, buf, len);
	}
	ele->stage1 = (j + 1) % 8;
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




int goslow_create(struct artery* ele, u8* url)
{
	say("@goslow_create\n");
	ele->stage1 = 0;
	return 1;
}
