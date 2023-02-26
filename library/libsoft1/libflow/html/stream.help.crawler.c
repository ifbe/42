#include "libsoft.h"




int crawler_read(_obj* art,void* foot, _syn* stack,int sp, u8* arg, int idx, void** buf, int len)
{
	say("@crawler_read\n");
	return 0;
}
int crawler_write(_obj* art,void* foot, _syn* stack,int sp, u8* arg, int idx, void* buf, int len)
{
	say("@crawler_write\n");
	printmemory(buf, len);
	return 0;
}
int crawler_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int crawler_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int crawler_delete(_obj* ele)
{
	say("@crawler_delete\n");
	return 0;
}
int crawler_create(_obj* ele, u8* arg)
{
	say("@crawler_create\n");
	return 1;
}
