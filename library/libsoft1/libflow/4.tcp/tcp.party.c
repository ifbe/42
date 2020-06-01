#include "libsoft.h"
#define PERBUF buf0
#define TO data2
#define BY data3
//
#define _sent_ hex32('s','e','n','t')
#define _done_ hex32('d','o','n','e')
//
#define _head_ hex32('h','e','a','d')
#define _data_ hex32('d','a','t','a')
void input(void* buf, int len);
int hexstr2u32(u8* src, u32* dat);




struct peruser{
	u32 sta_name;		//station name
	u32 sta_user;		//station user
	u32 dst_name;		//destination name
	u32 dst_user;		//destination user
	u32 sts;
	u32 len;
	u32 hash_all;		//whole packet
	u32 hash_cur;		//until now
	u8 head[0xe0];
};
int party_login(struct peruser* per, u32 name)
{
	int j;
	for(j=0;j<16;j++){
		if(0 == per[j].sta_name)goto found;
		if(name == per[j].sta_name)return -2;
	}
	return -1;

found:
	per[j].sta_name = name;
	per[j].sta_user = 0;
	per[j].dst_name = 0;
	per[j].dst_user = 0;
	per[j].sts = 0;
	per[j].len = 0;
	per[j].hash_all = 0;
	per[j].hash_cur = 0;

	per[j].head[0] = 0;
	return j;
}
int party_logout(struct peruser* per, u32 name)
{
	int j;
	for(j=0;j<16;j++){
		if(name == per[j].sta_name)goto found;
	}
	return -1;

found:
	per[j].sta_name = 0;
	return j;
}




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
static int party_sum(u8* buf, int len)
{
	int j,ret=0;
	for(j=0;j<len;j++)ret += buf[j];
	return ret;
}
static int party_parse(u8* buf, int len, u64* to, u64* by, u32* cnt, u32* sum)
{
	int j,k=0;
	for(j=0;j<len;j++){
		if( (',' == buf[j]) | (':' == buf[j]) ){
			if(0 == ncmp(buf+k, "to ", 3))to[0] = str2val(buf+k+3);
			if(0 == ncmp(buf+k, "by ", 3))by[0] = str2val(buf+k+3);
			if(0 == ncmp(buf+k, "cnt ", 4))hexstr2u32(buf+k+4, cnt);
			if(0 == ncmp(buf+k, "sum ", 4))hexstr2u32(buf+k+4, sum);

			if(':' == buf[j])return j+1;
			k = j+1;
		}
	}
	return 0;
}




int partyclient_write_sent(_syn* self,_syn* peer, _syn* stack,int sp, u8* buf, int len)
{
	struct artery* art = self->pchip;
	art->stage1 = _done_;

	//success or error
	if(0 == ncmp(buf, "HTTP", 4)){
		say("@partyclient error:<<<\n%.*s>>>\n",len,buf);
	}
	else{
		say("@partyclient success:<<<\n%.*s\n>>>\n",len,buf);
	}
	return 0;
}
int partyclient_write_done(_syn* self,_syn* peer, _syn* stack,int sp, u8* buf,int len)
{
	//printmemory(buf, len);
	struct artery* art = self->pchip;
	if(0 == art)return 0;
	struct peruser* per = art->PERBUF;
	if(0 == per)return 0;

while(1){
	if(0 == per->sts){		//no packet yet
		u64 to = 0;
		u64 by = 0;
		u32 cnt = 0;
		u32 sum = 0;
		int ret = party_parse(buf,len, &to, &by, &cnt, &sum);
		//say("to=%x,by=%x,cnt=%x,ret=%x,len=%x\n",to,by,cnt,ret,len);

		if(len == ret+cnt){		//complete packet
			give_data_into_peer(art,_dst_, stack,sp, 0,0, buf+ret,cnt);
			give_data_into_peer(art,_std_, stack,sp, 0,0, buf+ret,cnt);
			return 0;
		}
		if(len > ret+cnt){		//complete packet + next packet
			give_data_into_peer(art,_dst_, stack,sp, 0,0, buf+ret,cnt);
			give_data_into_peer(art,_std_, stack,sp, 0,0, buf+ret,cnt);
			buf += ret+cnt;
			len -= ret+cnt;
			continue;
		}
		if(len < ret+cnt){		//head ok, body not
			give_data_into_peer(art,_dst_, stack,sp, 0,0, buf+ret,len-ret);
			give_data_into_peer(art,_std_, stack,sp, 0,0, buf+ret,len-ret);
			per->dst_name = to;
			per->sts = _data_;
			per->len = cnt-(len-ret);
			return 0;
		}
	}
	//else if(_head_ == per->sts){	//head still not complete
	//}
	else if(_data_ == per->sts){		//data still not complete
		if(len == per->len){
			give_data_into_peer(art,_dst_, stack,sp, 0,0, buf,len);
			give_data_into_peer(art,_std_, stack,sp, 0,0, buf,len);
			per->sts = 0;
			return 0;
		}
		if(len < per->len){
			give_data_into_peer(art,_dst_, stack,sp, 0,0, buf,len);
			give_data_into_peer(art,_std_, stack,sp, 0,0, buf,len);
			per->len -= len;
			return 0;
		}
		if(len > per->len){
			give_data_into_peer(art,_dst_, stack,sp, 0,0, buf,per->len);
			give_data_into_peer(art,_std_, stack,sp, 0,0, buf,per->len);
			per->sts = 0;
			buf += per->len;
			len -= per->len;
			continue;
		}
	}
	else{
		systemdelete(peer->pchip);
		return 0;
	}
}
/*
	//parse,operate        to aaa by bbb:dat
	u64 to = 0;
	u64 by = 0;
	u32 cnt = 0;
	int ret = party_parse(buf,len, &to, &by, &cnt);
	say("to=%x,by=%x,cnt=%x,ret=%x,len=%x\n",to,by,cnt,ret,len);
	if(to != art->BY)ret = 0;

	give_data_into_peer(art,_std_, 0,0, buf+ret,len-ret);
	give_data_into_peer(art,_dst_, 0,0, buf+ret,len-ret);
	return 0;*/
}
int partyclient_write_bydst(_syn* self,_syn* peer, _syn* stack,int sp, u8* buf,int len)
{
	u8 tmp[64];
	if(len <= 0){
		say("len=%x\n", len);
		return 0;
	}

	struct artery* art = self->pchip;
	int ret = mysnprintf(tmp, 64, "to %.4s,by %.4s,cnt %x,sum %x:", &art->TO, &art->BY, len, party_sum(buf,len));
	give_data_into_peer(art, _src_, stack,sp, 0,0, tmp,ret);
	give_data_into_peer(art, _src_, stack,sp, 0,0, buf,len);
	return 0;
}
int partyclient_write_bystd(_syn* self,_syn* peer, _syn* stack,int sp, u8* buf,int len)
{
	u8 tmp[64];
	if(len <= 0){
		say("len=%x\n", len);
		return 0;
	}

	struct artery* art = self->pchip;
	int ret = mysnprintf(tmp, 64, "to %.4s,by %.4s,cnt 1,sum %x:", &art->TO, &art->BY,buf[0]);
	tmp[ret] = buf[0];
	give_data_into_peer(art, _src_, stack,sp, 0,0, tmp,ret+1);
	return 0;
}




int partyclient_write(_art* art,int foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	//say("@partyclient_write: %.4s\n", &self->flag);
	//printmemory(buf,len);

	if(0==stack|sp<2)return 0;
	struct halfrel* self = &stack[sp-1];
	struct halfrel* peer = &stack[sp-2];

	switch(self->flag){
	case _std_:partyclient_write_bystd(self,peer, stack,sp, buf,len);break;
	case _dst_:partyclient_write_bydst(self,peer, stack,sp, buf,len);break;
	case _src_:{
		if(_done_ == art->stage1){
			partyclient_write_done(self,peer, stack,sp, buf,len);
			break;
		}
		if(_sent_ == art->stage1){
			partyclient_write_sent(self,peer, stack,sp, buf,len);
			break;
		}
		say("error@partyclient_write\n");
		break;
	}
	}
	return 0;
}
int partyclient_read(_art* art,int foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
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
	struct sysobj* obj = peer->pchip;
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
		give_data_into_peer(art,_src_, 0,0, 0,0, tmp,ret);
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

	ele->PERBUF = memorycreate(0x1000, 0);
	ele->stage1 = 0;
	return 0;
}




int partyserver_write(_art* art,int foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	say("@partyserver_write\n");
	return 0;
}
int partyserver_read(_art* art,int foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
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




int partymaster_write_other(_syn* self,_syn* peer, _syn* stack,int sp, u8* buf, int len)
{
	say("@partymaster_write_other: foot=%.4s,len=%x\n", &self->flag, len);
	printmemory(buf, 0x20);

	struct artery* art = self->pchip;
	if(0 == art)return 0;
	struct peruser* per = self->pfoot;
	if(0 == per)return 0;
	//printmemory(per, 0x20);

while(1){
	say("sts=%x,len=%x\n", per->sts, per->len);
	printmemory(buf, 0x20);

	if(0 == per->sts){		//no packet yet
		u64 to = 0;
		u64 by = 0;
		u32 cnt = 0;
		u32 sum = 0;
		int ret = party_parse(buf,len, &to, &by, &cnt, &sum);
		say("to=%x,by=%x,cnt=%x,sum=%x,ret=%x,len=%x\n", to,by,cnt,sum, ret,len);

		if(ret < 8){
			give_data_into_peer(art,peer->flag, stack,sp, 0,0, "wrong head\n", 10);
			systemdelete(peer->pchip);
			return 0;
		}

		if(len == ret+cnt){		//complete packet
say("dbg1\n");
			give_data_into_peer(art,to, stack,sp, 0,0, buf,len);
			return 0;
		}
		if(len > ret+cnt){		//complete packet + next packet
say("dbg2\n");
			give_data_into_peer(art,to, stack,sp, 0,0, buf,ret+cnt);
			buf += ret+cnt;
			len -= ret+cnt;
			continue;
		}
		if(len < ret+cnt){		//head ok, body not
say("dbg3\n");
			give_data_into_peer(art,to, stack,sp, 0,0, buf,len);
			//per->sta_name = by;
			//per->sta_user = 0;
			per->dst_name = to;
			//per->dst_user = 0;
			per->sts = _data_;
			per->len = cnt-(len-ret);
			per->hash_all = sum;
			per->hash_cur = party_sum(buf+ret, len-ret);
			return 0;
		}
	}
	//else if(_head_ == per->sts){	//head still not complete
	//}
	else if(_data_ == per->sts){		//data still not complete
		if(len == per->len){
say("dbg4\n");
			give_data_into_peer(art,per->dst_name, stack,sp, 0,0, buf,len);
			per->hash_cur += party_sum(buf, len);
			if(per->hash_all != per->hash_cur){
				say("%x nequal %x\n", per->hash_all, per->hash_cur);
				systemdelete(peer->pchip);
			}

			per->sts = 0;
			return 0;
		}
		if(len > per->len){
say("dbg5\n");
			give_data_into_peer(art,per->dst_name, stack,sp, 0,0, buf,per->len);
			per->hash_cur += party_sum(buf, per->len);
			if(per->hash_all != per->hash_cur){
				say("%x nequal %x\n", per->hash_all, per->hash_cur);
				systemdelete(peer->pchip);
			}

			per->sts = 0;
			buf += per->len;
			len -= per->len;
			continue;
		}
		if(len < per->len){
say("dbg6\n");
			give_data_into_peer(art,per->dst_name, stack,sp, 0,0, buf,len);
			per->hash_cur += party_sum(buf, len);

			per->len -= len;
			return 0;
		}
	}
	else{
		say("error@partymaster_write_other:unknown per->sts\n");
		systemdelete(peer->pchip);
		return 0;
	}
}
/*
	//parse,transmit        to aaa,by bbb:dat

	if(0 == to){
		to = self->foot;
		if(0 == to)return 0;
	}
	else self->foot = to;
	say("to=%llx,by=%llx\n", to, by);

	give_data_into_peer(art,to, 0,0, buf,len);
*/
	return 0;
}
int partymaster_write_first(_syn* self,_syn* peer, _syn* stack,int sp, u8* buf, int len)
{
	int j,k,ret;
	u64 name;
	u8 tmp[256];
	struct artery* art = self->pchip;
	struct sysobj* sys = peer->pchip;

	struct peruser* per = art->PERBUF;
	if(0 == per)return 0;

	//if TCP error: return
	if(0 == sys->tempobj)return 0;

	//if not party: report state, close
	if(0 != ncmp(buf, "dear party:\n", 12))goto fail;

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
	//check valid name
	ret = party_login(per, name);
	if(ret < 0)goto fail;

	//if not connected: connect
	relationcreate(art, &per[ret], _art_, name, sys->tempobj, 0, _sys_, _dst_);

	//send status messge
	ret = mysnprintf(tmp, 128,
"dear %.4s:\n"
"login success!\n",
&name
	);
	give_data_into_peer(art,name, stack,sp, 0,0, tmp,ret);
	return 0;

fail:
	ret = mysnprintf(tmp, 128,
"HTTP/1.1 200 OK\r\n"
"\r\n"
"party client:\n"
	);
	for(j=0;j<16;j++){
		if(per[j].sta_name)ret += mysnprintf(
			tmp+ret, 250-ret,
			"%d: %.4s\n", j, &per[j].sta_name
		);
	}
	give_data_into_peer(art,self->flag, stack,sp, 0,0, tmp,ret);
	systemdelete(sys->tempobj);
	return 0;
}




int partymaster_write(_art* art,int foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	//say("@partymaster_write\n");
	if(0==stack|sp<2)return 0;
	struct halfrel* self = &stack[sp-1];
	struct halfrel* peer = &stack[sp-2];
	struct sysobj* sys = peer->pchip;

	//printmemory(buf, len);
	//say("valid message:%.*s", len, buf);

	//only handle socket
	if(_sys_ != sys->tier)return 0;

	//new or old
	switch(sys->type){
	case _TCP_:return partymaster_write_first(self,peer, stack,sp, buf,len);
	case _Tcp_:return partymaster_write_other(self,peer, stack,sp, buf,len);
	}
	return 0;
}
int partymaster_read(_art* art,int foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int partymaster_discon(struct halfrel* self, struct halfrel* peer)
{
	say("@partymaster_discon\n");
	struct artery* art = self->pchip;
	struct peruser* per = art->PERBUF;
	party_logout(per, self->flag);
	return 0;
}
int partymaster_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@partymaster_linkup\n");
	return 0;
}
int partymaster_delete(struct artery* ele)
{
	return 0;
}
int partymaster_create(struct artery* ele, u8* url)
{
	ele->PERBUF = memorycreate(0x100000, 0);
	return 0;
}
