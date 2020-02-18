#include "libsoft.h"
#define _sent_ hex32('s','e','n','t')
#define _done_ hex32('d','o','n','e')
void input(void* buf, int len);




int partyclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	say("@partyclient_write\n");
	printmemory(buf,len);

	struct artery* art = self->pchip;
	switch(art->stage1){
	case _sent_:{
		//relationwrite(art,self->flag, 0,0, "test\n",5);
		art->stage1 = _done_;
		break;
	}
	case _done_:{
		input(buf, len);
		break;
	}
	default:say("error@partyclient_write\n");break;
	}

	//relationwrite(art, _src_, 0,0, "partyclient!\n", 0);
	return 0;
}
int partyclient_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int partyclient_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int partyclient_linkup(struct halfrel* self, struct halfrel* peer)
{
	struct artery* art = self->pchip;
	struct object* obj = peer->pchip;
	if( (_sys_ == obj->tier) && (_tcp_ == obj->type) ){
		struct artery* art = self->pchip;
		relationwrite(art,_src_, 0,0, "dear party:\nI volunteer to join\n", 32);
		art->stage1 = _sent_;
	}
	return 0;
}
int partyclient_delete(struct artery* ele)
{
	return 0;
}
int partyclient_create(struct artery* ele, u8* url)
{
	ele->stage1 = 0;
	return 0;
}




int partyserver_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	struct artery* art = self->pchip;
	say("@partyserver_write\n");

	relationwrite(art, _src_, 0,0, "partyserver!\n", 0);
	return 0;
}
int partyserver_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int partyserver_delete(struct artery* ele)
{
	return 0;
}
int partyserver_create(struct artery* ele, u8* url)
{
	return 0;
}




int partymaster_write_first(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	int ret;
	u8 tmp[128];
	struct artery* art = self->pchip;
	struct object* sys = peer->pchip;

	//if not party: report state, close
	if(0 != ncmp(buf, "dear party:\n", 12)){
		ret = mysnprintf(tmp, 128,
"HTTP/1.1 200 OK\r\n"
"\r\n"
"party only!\n"
		);
		relationwrite(art, self->flag, 0,0, tmp,ret);
		systemdelete(sys->tempobj);
		return 0;
	}

	//if not connected: connect
	struct object* Tcp = sys->tempobj;
	if(0 == Tcp)return 0;

	//parse name from packet
	u32 name = 'c';
	relationcreate(art, 0, _art_, name, Tcp, 0, _sys_, _dst_);

	ret = mysnprintf(tmp, 128,
"dear %.4s:\n"
"ok!\n",
&name
	);
	relationwrite(art, name, 0,0, tmp, ret);
	return 0;
}
int partymaster_write_other(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	say("@partymaster_write_other: %.4s\n", &self->flag);
	struct artery* art = self->pchip;
	relationwrite(art, self->flag, 0,0, "ls\n", 3);
	return 0;
}
int partymaster_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	//say("@partymaster_write\n");
	struct artery* art = self->pchip;
	struct object* sys = peer->pchip;
	printmemory(buf, len);
	//say("valid message:%.*s", len, buf);

	//only handle socket
	if(_sys_ == sys->tier){
		switch(sys->type){
		case _TCP_:return partymaster_write_first(self,peer, buf,len);
		case _Tcp_:return partymaster_write_other(self,peer, buf,len);
		}
	}
	return 0;
}
int partymaster_read( struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int partymaster_delete(struct artery* ele)
{
	return 0;
}
int partymaster_create(struct artery* ele, u8* url)
{
	return 0;
}
