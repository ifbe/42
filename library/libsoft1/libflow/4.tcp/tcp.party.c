#include "libsoft.h"
#define TO data0
#define BY data1
#define _sent_ hex32('s','e','n','t')
#define _done_ hex32('d','o','n','e')
void input(void* buf, int len);
static u64 str2val(u8* str)
{
	int j;
	u64 dat = 0;
	u8* tmp = (u8*)&dat;
	for(j=0;j<4;j++){
		if( (str[j] >= '0' && str[j] <= '9') |
		    (str[j] >= 'a' && str[j] <= 'z') |
			(str[j] >= 'A' && str[j] <= 'Z') |
			(str[j] == '_') )
		{
			tmp[j] = str[j];
		}
		else break;
	}
	return dat;
}
int parse_toby(u8* buf, int len, u64* to, u64* by)
{
	int j,k=0;
	for(j=0;j<len;j++){
		if( (',' == buf[j]) | (':' == buf[j]) ){
			if(0 == ncmp(buf+k, "to ", 3))to[0] = str2val(buf+k+3);
			if(0 == ncmp(buf+k, "by ", 3))by[0] = str2val(buf+k+3);

			if(':' == buf[j])return j+1;
			k = j+1;
		}
	}
	return 0;
}




int partyclient_write_bynet(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	struct artery* art = self->pchip;
	switch(art->stage1){
	case _sent_:{
		//server first reply: tell me if any thing wrong
		//relationwrite(art,self->flag, 0,0, "test\n",5);
		art->stage1 = _done_;
		break;
	}
	case _done_:{
		//parse,operate        to aaa by bbb:dat
		//relationwrite(art,_dst_, 0,0, dat,len);

		u64 to = 0;
		u64 by = 0;
		int ret = parse_toby(buf,len, &to, &by);
		if(to != art->BY)return 0;

		//input(buf+ret, len-ret);
		relationwrite(art,_std_, 0,0, buf+ret,len-ret);
		relationwrite(art,_dst_, 0,0, buf+ret,len-ret);
		break;
	}
	default:say("error@partyclient_write\n");break;
	}
	return 0;
}
int partyclient_write_bydst(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	int ret;
	u8 tmp[64];
	struct artery* art = self->pchip;

	ret = mysnprintf(tmp, 32, "to %.4s,by %.4s:", &art->TO, &art->BY);
	relationwrite(art, _src_, 0,0, tmp,ret);
	relationwrite(art, _src_, 0,0, buf,len);
	return 0;
}
int partyclient_write_bystd(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	int ret;
	u8 tmp[64];
	struct artery* art = self->pchip;

	ret = mysnprintf(tmp, 32, "to %.4s,by %.4s:", &art->TO, &art->BY);
	tmp[ret] = buf[0];
	relationwrite(art, _src_, 0,0, tmp,ret+1);

	return 0;
}




int partyclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	//say("@partyclient_write: %.4s\n", &self->flag);
	//printmemory(buf,len);

	switch(self->flag){
	case _std_:partyclient_write_bystd(self,peer, buf,len);break;
	case _dst_:partyclient_write_bydst(self,peer, buf,len);break;
	case _src_:partyclient_write_bynet(self,peer, buf,len);break;
	}
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
		int ret;
		u8 tmp[64];
		ret = mysnprintf(tmp,64,
"dear party:\n"
"I volunteer to join\n"
"myid %.4s\n"
"your %.4s\n",
&art->BY,
&art->TO
		);
		relationwrite(art,_src_, 0,0, tmp, ret);
		art->stage1 = _sent_;
	}
	return 0;
}
int partyclient_delete(struct artery* ele)
{
	return 0;
}
int partyclient_create(struct artery* ele, void* arg, int argc, u8** argv)
{
	int j;
	u64 to = 0;
	u64 by = 0;
	for(j=1;j<argc;j++){
		if(0 == ncmp(argv[j], "to:", 3))to = str2val(argv[j]+3);
		if(0 == ncmp(argv[j], "by:", 3))by = str2val(argv[j]+3);
	}
	say("to=%llx,by=%llx\n", to, by);

	ele->TO = to;
	ele->BY = by;
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




int partymaster_write_other(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	say("@partymaster_write_other: %.4s\n", &self->flag);
	struct artery* art = self->pchip;

	//parse,transmit        to aaa,by bbb:dat
	u64 to = 0;
	u64 by = 0;
	int ret = parse_toby(buf,len, &to, &by);

	if(0 == to){
		to = self->foot;
		if(0 == to)return 0;
	}
	else self->foot = to;
	say("to=%llx,by=%llx\n", to, by);

	relationwrite(art,to, 0,0, buf,len);
	return 0;
}
int partymaster_write_first(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	int j,k,ret;
	u64 name;
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

	//parse name from packet
	k = 12;
	name = 0;
	for(j=12;j<len;j++){
		if('\n' != buf[j])continue;
		if(0 == ncmp(buf+k, "myid ", 5)){
			name = str2val(buf+k+5);
			goto found;
		}
		k = j+1;
	}
	return 0;

found:
	//if not connected: connect
	if(0 == sys->tempobj)return 0;
	relationcreate(art, 0, _art_, name, sys->tempobj, 0, _sys_, _dst_);

	//send status messge
	ret = mysnprintf(tmp, 128,
"dear %.4s:\n"
"ok!\n",
&name
	);
	relationwrite(art, name, 0,0, tmp, ret);
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
