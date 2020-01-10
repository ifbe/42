#include "libsoft.h"




int dbghex_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int dbghex_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
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
