#include "libsoft.h"
#define _src_ hex32('s','r','c',0)
#define _dst_ hex32('d','s','t',0)




int reorder_read(_art* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	say("@reorder_read\n");

	float f[10];
	take_data_from_peer(art,_src_, stack,sp, 0,0, f,10);
	return 0;
}
int reorder_write(_art* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	say("@reorder_write:%d\n", len);
	return 0;
}
int reorder_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int reorder_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int reorder_create(struct artery* ele, u8* url)
{
	say("@reorder_create\n");
	return 1;
}
