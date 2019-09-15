#include "libsoft.h"




int dbglog_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int dbglog_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	say("@dbglog_write: 0x%x\n", len);
	printmemory(buf, len<16 ? len : 16);
	return 0;
}
int dbglog_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int dbglog_start(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int dbglog_create(struct element* ele, u8* url)
{
	say("@dbglog_create\n");
	return 1;
}
