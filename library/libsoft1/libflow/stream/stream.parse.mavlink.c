#include "libsoft.h"




int mavlinkclient_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int mavlinkclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	printmemory(buf, len);
	return 0;
}
int mavlinkclient_delete(struct element* ele)
{
	return 0;
}
int mavlinkclient_create(struct element* ele, u8* url)
{
	say("@mavlinkclient_create\n");
	return 0;
}





int mavlinkserver_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int mavlinkserver_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int mavlinkserver_delete(struct element* ele)
{
	return 0;
}
int mavlinkserver_create(struct element* ele, u8* url)
{
	say("@mavlinkserver_create\n");
	return 0;
}
