#include "libsoft.h"




int crawler_read(_obj* art,void* foot, _syn* stack,int sp, u8* arg, int idx, void** buf, int len)
{
	logtoall("@crawler_read\n");
	return 0;
}
int crawler_write(_obj* art,void* foot, _syn* stack,int sp, u8* arg, int idx, void* buf, int len)
{
	logtoall("@crawler_write\n");
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
	logtoall("@crawler_delete\n");
	return 0;
}
int crawler_create(_obj* ele, u8* arg)
{
	logtoall("@crawler_create\n");
	return 1;
}
