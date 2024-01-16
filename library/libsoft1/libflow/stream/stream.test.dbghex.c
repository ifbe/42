#include "libsoft.h"




int dbghex_read(_obj* art,void* foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
	return 0;
}
int dbghex_write(_obj* art,void* foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
	logtoall("@dbghex_write: 0x%x\n", len);
	printmemory(buf, len<16 ? len : 16);
	return 0;
}
int dbghex_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int dbghex_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int dbghex_create(_obj* ele, u8* arg)
{
	logtoall("@dbghex_create\n");
	return 1;
}
