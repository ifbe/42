#include "libsoft.h"




int bootpclient_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int bootpclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int bootpclient_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int bootpclient_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int bootpclient_delete(struct artery* ele)
{
	return 0;
}
int bootpclient_create(struct artery* ele, u8* url)
{
	return 0;
}




int bootpserver_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int bootpserver_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int bootpserver_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int bootpserver_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int bootpserver_delete(struct artery* ele)
{
	return 0;
}
int bootpserver_create(struct artery* ele, u8* url)
{
	return 0;
}
