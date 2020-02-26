#include "libsoft.h"




int stunclient_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int stunclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int stunclient_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int stunclient_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int stunclient_delete(struct artery* ele)
{
	return 0;
}
int stunclient_create(struct artery* ele, u8* url)
{
	return 0;
}




int stunserver_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int stunserver_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int stunserver_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int stunserver_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int stunserver_delete(struct artery* ele)
{
	return 0;
}
int stunserver_create(struct artery* ele, u8* url)
{
	return 0;
}




int stunmaster_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int stunmaster_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	struct artery* art = self->pchip;
	struct object* sys = peer->pchip;
	if( (_UDP_ == sys->type) | (_udp_ == sys->type) ) {
		if(arg){
			u8* tmp = arg;
			say("by %d.%d.%d.%d:%d\n",
			tmp[4],tmp[5],tmp[6],tmp[7],
			(tmp[2]<<8)+tmp[3]
			);
		}
		printmemory(buf, len);
	}
	return 0;
}
int stunmaster_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@stunmaster_linkup: %.4s\n", &self->flag);
	return 0;
}
int stunmaster_discon(struct halfrel* self, struct halfrel* peer)
{
	say("@stunmaster_discon: %.4s\n", &self->flag);
	return 0;
}
int stunmaster_delete(struct artery* ele)
{
	return 0;
}
int stunmaster_create(struct artery* ele, u8* url)
{
	return 0;
}