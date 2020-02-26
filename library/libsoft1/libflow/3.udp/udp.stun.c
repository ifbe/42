#include "libsoft.h"
u32 resolvehostname(void*);




int stunclient_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int stunclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	struct artery* art = self->pchip;
	if(_std_ == self->flag){
		int j;
		u8 tmp[16];
		for(j=0;j<16;j++)tmp[j] = 0;
		tmp[0] = 0x10;
		tmp[1] = 0x02;
		tmp[2] = 9999>>8;
		tmp[3] = 9999&0xff;
		*(u32*)(tmp+4) = art->data0;
	}
	else{
		printmemory(buf,len);
	}
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
int stunclient_delete(struct artery* art)
{
	return 0;
}
int stunclient_create(struct artery* art, u8* url)
{
	art->data0 = resolvehostname(url);
	say("ip = %x\n", art->data0);
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
int stunserver_delete(struct artery* art)
{
	return 0;
}
int stunserver_create(struct artery* art, u8* url)
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
			relationwrite(art,self->flag, arg,idx, arg,8);
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
int stunmaster_delete(struct artery* art)
{
	return 0;
}
int stunmaster_create(struct artery* art, u8* url)
{
	return 0;
}