#include "libsoft.h"
int decstr2u32(void* src, void* dst);
#define CURNUM data0
#define MAXNUM data1




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
	int cur,max;
	struct artery* ele = self->pchip;
	if(0 == ele)return 0;

	if(_src_ == self->flag){
		cur = ele->CURNUM;
		max = ele->MAXNUM;
		say("@goslow_write:%d/%d\n", cur, max);

		if(0 == cur){
			say("@goslow.sending\n");
			relationwrite(ele, _dst_, 0, 0, buf, len);
		}
		ele->CURNUM = (cur + 1) % max;
	}
	return 0;
}
int goslow_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int goslow_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int goslow_create(struct artery* ele, u8* url)
{
	ele->CURNUM = 0;
	ele->MAXNUM = 8;	//default
	if(url)decstr2u32(url, &ele->MAXNUM);

	say("@goslow_create:%d\n",ele->MAXNUM);
	return 1;
}
