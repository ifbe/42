#include "libsoft.h"




int echo_read(_art* art,int foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	say("@echo_read\n");

	float f[10];
	relationread(art,_src_, stack,sp, 0,0, f,10);
	return 0;
}
int echo_write(_art* art,int foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	say("@echo_write\n");
	return relationwrite(art,_src_, stack,sp, 0,0, buf,len);
}
int echo_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int echo_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int echo_create(struct artery* ele, u8* url)
{
	say("@echo_create\n");
	return 1;
}
