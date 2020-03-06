#include "libsoft.h"




int vncclient_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int vncclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	say("@vncclient_write:%.4s\n",&self->flag);
	printmemory(buf, len);
	return 0;
}
int vncclient_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int vncclient_linkup(struct halfrel* self, struct halfrel* peer)
{
	int len;
	u8* buf[32];
	if(_src_ == self->flag){
		len = mysnprintf(buf,32, "RFB 003.008\n");
		relationwrite(self->pchip,_src_, 0,0, buf,len);
	}
	return 0;
}
int vncclient_delete(struct artery* art)
{
	return 0;
}
int vncclient_create(struct artery* art, void* arg)
{
	return 0;
}




int vncserver_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int vncserver_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int vncserver_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int vncserver_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int vncserver_delete(struct artery* art)
{
	return 0;
}
int vncserver_create(struct artery* art, void* arg)
{
	return 0;
}




int vncmaster_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int vncmaster_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int vncmaster_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int vncmaster_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int vncmaster_delete(struct artery* art)
{
	return 0;
}
int vncmaster_create(struct artery* art, void* arg)
{
	return 0;
}