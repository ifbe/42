#include "libsoft.h"
int sleep_us(int);
int modifysocket(int fd, int arg, void* addr, int port);
int createsocket_tcpclient(void* myaddr, int myport, void* toaddr, int toport);
//
#define _ccc_ hex32('c','c','c', 0)
#define _sss_ hex32('s','s','s', 0)
#define _connect_ hex32('c','o','n', 0)
#define _listen_ hex32('l','i','s', 0)
//
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
	struct artery* art = self->pchip;
	struct object* sys = peer->pchip;
	say("p2pclient_write:%.4s\n", &self->flag);

	if(_dst_ == self->flag){
		return 0;
	}
	if(_ccc_ == self->flag){
		say("@ccc\n");
		return 0;
	}
	if(_sss_ == self->flag){
		say("@sss\n");
		return 0;
	}
	if(_std_ == self->flag){
		printmemory(buf, len);
		if(' ' == buf[0])relationwrite(self->pchip, _src_, 0,0, buf, 1);
		if('1' == buf[1])relationwrite(self->pchip, _ccc_, 0,0, buf, 1);
		if('2' == buf[1])relationwrite(self->pchip, _sss_, 0,0, buf, 1);
		return 0;
	}
	if(_src_ == self->flag){
		printmemory(buf,len);

		u8* t = sys->self;
		say("myaddr=%d.%d.%d.%d:%d\n", t[4],t[5],t[6],t[7], (t[2]<<8)+t[3]);
		if(len < 16)return 0;

		u8* p = buf+0;
		say("public=%d.%d.%d.%d:%d\n", p[4],p[5],p[6],p[7], (p[2]<<8)+p[3]);

		if(len < 16)return 0;
		if(_c_friend_ == art->stage1)return 0;

		u8* r = buf+8;
		say("remote=%d.%d.%d.%d:%d\n", r[4],r[5],r[6],r[7], (r[2]<<8)+r[3]);

		int j;
		char* tmp[64];

		//connect
		struct system* ccc;
		mysnprintf(tmp,64,
			"%d.%d.%d.%d:%d->%d.%d.%d.%d:%d",
			t[4],t[5],t[6],t[7], (t[2]<<8)+t[3],
			r[4],r[5],r[6],r[7], (r[2]<<8)+r[3]
		);
		for(j=0;j<10;j++){
			ccc = systemcreate(_tcp_, tmp, 0, 0);
			say("ccc=%llx\n",ccc);
			if(ccc){
				relationcreate(art, 0, _art_, _ccc_, ccc, 0, _sys_, _dst_);
				break;
			}
			sleep_us(1000*1000);
		}

		//listen
		struct system* sss;
		mysnprintf(tmp,32,
			"%d.%d.%d.%d:%d",
			t[4],t[5],t[6],t[7], (t[2]<<8)+t[3]
		);
		sss = systemcreate(_TCP_, tmp, 0, 0);
		if(sss){
			relationcreate(art, 0, _art_, _sss_, sss, 0, _sys_, _dst_);
		}

		art->stage1 = _c_friend_;
/*
		//debug
		char* tmp[32];
		t = sys->self;
		mysnprintf(tmp, 32, "%d.%d.%d.%d", t[4],t[5],t[6],t[7]);
		int fd = createsocket_tcpclient(tmp, (t[2]<<8)+t[3], "127.0.0.1", 8888);
		write(fd, "debug\n", 6);*/
	}
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