#include "libsoft.h"




int dbglog_read(struct halfrel* self, struct halfrel* peer, void* buf, int len)
{
	return 0;
}
int dbglog_write(struct halfrel* self, struct halfrel* peer, void* buf, int len)
{
	say("@dbglog_write: %llx, %x\n", buf, len);
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
