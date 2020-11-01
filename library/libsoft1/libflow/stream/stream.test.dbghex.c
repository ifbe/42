#include "libsoft.h"




int dbghex_read(struct artery* art,void* foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
	return 0;
}
int dbghex_write(struct artery* art,void* foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
	say("@dbghex_write: 0x%x\n", len);
	printmemory(buf, len<16 ? len : 16);
	return 0;
}
int dbghex_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int dbghex_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int dbghex_create(struct artery* ele, u8* url)
{
	say("@dbghex_create\n");
	return 1;
}
