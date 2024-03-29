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




int tcptrav_memory(u64* list, u64 self)
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




int tcptravclient_read(_obj* art,void* foot, _syn* stack,int sp, void* arg,int idx, u8* buf,int len)
{
	return 0;
}
int tcptravclient_write(_obj* art,void* foot, _syn* stack,int sp, void* arg,int idx, u8* buf,int len)
{
	if(0==stack|sp < 2)return 0;
	_obj* sys = stack[sp-2].pchip;
	logtoall("@tcptravclient_write:%.4s\n", &foot);

	switch(stack[sp-1].foottype){
	case _std_:
		printmemory(buf, len < 16 ? len : 16);

		if(' ' == buf[0])give_data_into_peer(art,_src_, stack,sp, 0,0, buf,1);
		if((buf[0]>='0') && (buf[0]<='9'))give_data_into_peer(art,_sss_, stack,sp, 0,0, buf,1);
		if((buf[0]>='a') && (buf[0]<='z'))give_data_into_peer(art,_ccc_, stack,sp, 0,0, buf,1);
		return 0;
	case _ccc_:
		///logtoall("@ccc\n");
		printmemory(buf, len < 16 ? len : 16);

		if(_c_friend_ != art->vfmt)return 0;
		give_data_into_peer(art,_dst_, stack,sp, 0,0, buf,len);
		return 0;
	case _sss_:
		//logtoall("@sss\n");
		printmemory(buf, len < 16 ? len : 16);
		return 0;
	case _dst_:
		if(_c_friend_ != art->vfmt)return 0;
		give_data_into_peer(art,_ccc_, stack,sp, 0,0, buf,len);
		return 0;
	case _src_:
		printmemory(buf, len < 16 ? len : 16);

		u8* t = sys->sockinfo.self;
		logtoall("myaddr=%d.%d.%d.%d:%d\n", t[4],t[5],t[6],t[7], (t[2]<<8)+t[3]);

		u8* p = buf+0;
		logtoall("public=%d.%d.%d.%d:%d\n", p[4],p[5],p[6],p[7], (p[2]<<8)+p[3]);

		if(_c_friend_ == art->vfmt)return 0;
		if(len < 16){
			sleep_us(1000000);
			give_data_into_peer(art,_src_, stack,sp, 0,0, "?\n", 2);
			return 0;
		}

		int j;
		char* tmp[64];
		u8* r = buf+8;
		logtoall("remote=%d.%d.%d.%d:%d\n", r[4],r[5],r[6],r[7], (r[2]<<8)+r[3]);

		//connect
		_obj* ccc;
		mysnprintf(tmp,64,
			"%d.%d.%d.%d:%d->%d.%d.%d.%d:%d",
			t[4],t[5],t[6],t[7], (t[2]<<8)+t[3],
			r[4],r[5],r[6],r[7], (r[2]<<8)+r[3]
		);
		for(j=0;j<10;j++){
			ccc = system_alloc_frompath(_tcp_, (u8*)tmp);
			system_create(ccc, tmp, 0, 0);
			logtoall("ccc=%llx\n",ccc);
			if(ccc){
				relationcreate(art, 0, _art_, _ccc_, ccc, 0, _sys_, _dst_);
				break;
			}
			sleep_us(1000*1000);
		}

		//listen
		_obj* sss;
		mysnprintf(tmp,32,
			"%d.%d.%d.%d:%d",
			t[4],t[5],t[6],t[7], (t[2]<<8)+t[3]
		);
		sss = system_alloc_frompath(_TCP_, (u8*)tmp);
		if(sss){
			system_create(sss, tmp, 0, 0);
			relationcreate(art, 0, _art_, _sss_, sss, 0, _sys_, _dst_);
		}

		art->vfmt = _c_friend_;
/*
		//debug
		char* tmp[32];
		t = sys->self;
		mysnprintf(tmp, 32, "%d.%d.%d.%d", t[4],t[5],t[6],t[7]);
		int fd = createsocket_tcpclient(tmp, (t[2]<<8)+t[3], "127.0.0.1", 8888);
		write(fd, "debug\n", 6);*/
		break;
	}
	return 0;
}
int tcptravclient_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int tcptravclient_attach(struct halfrel* self, struct halfrel* peer)
{
	logtoall("@tcptravclient_attach: %.4s\n", &self->foottype);
	if(_src_ == self->foottype){
		give_data_into_peer(self->pchip, self->foottype, 0,0, 0,0, "?\n", 2);
	}
	return 0;
}
int tcptravclient_delete(_obj* art)
{
	return 0;
}
int tcptravclient_create(_obj* art, u8* url)
{
	art->listptr.buf0 = 0;
	art->vfmt = 0;
	return 0;
}




int tcptravserver_read(_obj* art,void* foot, _syn* stack,int sp, void* arg,int idx, u8* buf,int len)
{
	return 0;
}
int tcptravserver_write(_obj* art,void* foot, _syn* stack,int sp, void* arg,int idx, u8* buf,int len)
{
	return 0;
}
int tcptravserver_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int tcptravserver_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int tcptravserver_delete(_obj* art)
{
	return 0;
}
int tcptravserver_create(_obj* art, u8* url)
{
	return 0;
}




int tcptravmaster_read(_obj* art,void* foot, _syn* stack,int sp, p64 arg,int idx, u8* buf,int len)
{
	return 0;
}
int tcptravmaster_write(_obj* art,void* foot, _syn* stack,int sp, p64 arg,int idx, u8* buf,int len)
{
	if(0==stack|sp < 2)return 0;
	_obj* sys = stack[sp-2].pchip;

	if(_TCP_ == sys->type){
		u8* t = sys->sockinfo.peer;
		logtoall("from %d.%d.%d.%d:%d->\n", t[4],t[5],t[6],t[7], (t[2]<<8)+t[3]);
		printmemory(buf, len);

		u64* list = art->listptr.buf0;
		tcptrav_memory(list, *(u64*)t);

		int j;
		for(j=0;j<4;j++){
			if(0 == list[j])break;
		}
		give_data_into_peer(art,stack[sp-1].foottype, stack,sp, arg,idx, list,j*8);
	}
	return 0;
}
int tcptravmaster_attach(struct halfrel* self, struct halfrel* peer)
{
	logtoall("@tcptravmaster_attach: %.4s\n", &self->foottype);
	return 0;
}
int tcptravmaster_detach(struct halfrel* self, struct halfrel* peer)
{
	logtoall("@tcptravmaster_detach: %.4s\n", &self->foottype);
	return 0;
}
int tcptravmaster_delete(_obj* art)
{
	return 0;
}
int tcptravmaster_create(_obj* art, u8* url)
{
	int j;
	u64* list = art->listptr.buf0;
	for(j=0;j<4;j++)list[j] = 0;
	return 0;
}
