#include "libsoft.h"




int crawler_read(_art* art,void* foot, _syn* stack,int sp, u8* arg, int idx, void** buf, int len)
{
	say("@crawler_read\n");
	return 0;
}
int crawler_write(_art* art,void* foot, _syn* stack,int sp, u8* arg, int idx, void* buf, int len)
{
	say("@crawler_write\n");
	printmemory(buf, len);
	return 0;
}
int crawler_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int crawler_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int crawler_delete(struct artery* ele)
{
	say("@crawler_delete\n");
	return 0;
}
int crawler_create(struct artery* ele, u8* url)
{
	say("@crawler_create\n");
	return 1;
}
