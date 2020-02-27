#include "libsoft.h"
#define _c_recved_ hex32('c','_','r', 0)
#define _c_friend_ hex32('c','_','f', 0)
#define _c_client_ hex32('c','_','c', 0)
#define _c_server_ hex32('c','_','s', 0)




int p2p_memory(u64* list, u64 self)
{
	int j;
	if(list[0] == self)return 0;
	if(0 == list[0]){
		list[0] = self;
		return 0;
	}

	for(j=0;j<3;j++){
		if(list[j] == self)break;
	}
	for(;j>0;j--)list[j] = list[j-1];
	list[0] = self;
	return 0;
}




int p2pclient_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int p2pclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	say("p2pclient_write\n");
	printmemory(buf,len);
	return 0;
}
int p2pclient_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int p2pclient_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@p2pclient_linkup: %.4s\n", &self->flag);
	if(_src_ == self->flag){
		relationwrite(self->pchip, self->flag, 0,0, "?\n", 2);
	}
	return 0;
}
int p2pclient_delete(struct artery* art)
{
	return 0;
}
int p2pclient_create(struct artery* art, u8* url)
{
	art->data0 = 0;
	art->data1 = 0;

	art->stage1 = 0;
	return 0;
}




int p2pserver_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int p2pserver_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int p2pserver_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int p2pserver_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int p2pserver_delete(struct artery* art)
{
	return 0;
}
int p2pserver_create(struct artery* art, u8* url)
{
	return 0;
}




int p2pmaster_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int p2pmaster_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	struct artery* art = self->pchip;
	struct object* sys = peer->pchip;
	if(_TCP_ == sys->type){
		u8* t = sys->peer;
		say("from %d.%d.%d.%d:%d->\n", t[4],t[5],t[6],t[7], (t[2]<<8)+t[3]);
		printmemory(buf, len);

		u64* list = &art->data0;
		p2p_memory(list, *(u64*)t);

		int j;
		for(j=0;j<4;j++){
			if(0 == list[j])break;
		}
		relationwrite(art,self->flag, arg,idx, list,j*8);
	}
	return 0;
}
int p2pmaster_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@p2pmaster_linkup: %.4s\n", &self->flag);
	return 0;
}
int p2pmaster_discon(struct halfrel* self, struct halfrel* peer)
{
	say("@p2pmaster_discon: %.4s\n", &self->flag);
	return 0;
}
int p2pmaster_delete(struct artery* art)
{
	return 0;
}
int p2pmaster_create(struct artery* art, u8* url)
{
	int j;
	u64* list = &art->data0;
	for(j=0;j<4;j++)list[j] = 0;
	return 0;
}