#include "libsoft.h"
u32 resolvehostname(void*);




int stunclient_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int stunclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	struct artery* art = self->pchip;
	//say("@stunclient_write: %.4s\n", &self->flag);

	if(_std_ == self->flag){
		u8 tmp[16];
		tmp[2] = 9999>>8;
		tmp[3] = 9999&0xff;
		*(u32*)(tmp+4) = art->data0;
		relationwrite(art,_src_, tmp,0, tmp, 8);
	}
	else{
		u8* t;
		t = arg;
		say("server: %d.%d.%d.%d:%d\n", t[4],t[5],t[6],t[7], (t[2]<<8)+t[3]);
		t = buf;
		say("myself: %d.%d.%d.%d:%d\n", t[4],t[5],t[6],t[7], (t[2]<<8)+t[3]);

		if(len < 16)return 0;
		t = buf+8;
		say("friend: %d.%d.%d.%d:%d\n", t[4],t[5],t[6],t[7], (t[2]<<8)+t[3]);
	}
	return 0;
}
int stunclient_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int stunclient_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@stunclient_linkup: %.4s\n", &self->flag);
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
		if(0 == arg){
			say("error@stunmaster_write\n");
			return 0;
		}

		u8* t = arg;
		say("%d.%d.%d.%d:%d: %.*s\n", t[4],t[5],t[6],t[7], (t[2]<<8)+t[3], len,buf);

		u8 out[64];
		*(u64*)out = *(u64*)arg;

		int j,k;
		u64* list = &art->data0;
		for(k=0;k<4;k++){
			if(0 == list[k]){
				list[k] = *(u64*)arg;
				break;
			}
			if(list[k] != *(u64*)t){
				*(u64*)(out+j) = *(u64*)list[k];
				j += 8;
			}
		}
		relationwrite(art,self->flag, arg,idx, out,j);
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
	int j;
	u64* list = &art->data0;
	for(j=0;j<4;j++)list[j] = 0;
	return 0;
}