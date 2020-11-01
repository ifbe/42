#include "libsoft.h"
int decstr2u32(void* src, void* dst);
#define CURNUM data0
#define MAXNUM data1




int goslow_read(_art* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	say("@goslow_read\n");

	float f[10];
	take_data_from_peer(art,_src_, stack,sp, 0,0, f,10);
	return 0;
}
int goslow_write(_art* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	int cur,max;
	if(0 == art)return 0;

	if(_src_ == stack[sp-1].flag){
		cur = art->CURNUM;
		max = art->MAXNUM;
		say("@goslow_write:%d/%d\n", cur, max);

		if(0 == cur){
			say("@goslow.sending\n");
			give_data_into_peer(art,_dst_, stack,sp, 0,0, buf,len);
		}
		art->CURNUM = (cur + 1) % max;
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
