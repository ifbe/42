#include "libsoft.h"




int reliableclient_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int reliableclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int reliableclient_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int reliableclient_delete(struct artery* art)
{
	return 0;
}
int reliableclient_create(struct artery* art, u8* url)
{
	return 0;
}




int reliableserver_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int reliableserver_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int reliableserver_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int reliableserver_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int reliableserver_delete(struct artery* art)
{
	return 0;
}
int reliableserver_create(struct artery* art, u8* url)
{
	return 0;
}




int reliablemaster_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int reliablemaster_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int reliablemaster_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int reliablemaster_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int reliablemaster_delete(struct artery* art)
{
	return 0;
}
int reliablemaster_create(struct artery* art, u8* url)
{
	return 0;
}
