#include "libsoft.h"
#define _ok_ hex16('o','k')




static u8 proxy_client0[] =
"CONNECT baidu.com:80 HTTP/1.1\r\n"
"\r\n";
static u8 proxy_server0[] =
"HTTP/1.1 200 Connection established\r\n"
"\r\n";
struct perobj{
	u64 len;
	u8* buf;

	u64 dlen;
	u8 data[0x80];
};




int proxyclient_read(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int proxyclient_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	say("@proxyclient_write: %llx, %.4s, %d\n", art, &foot, len);
	printmemory(buf, len<16?len:16);

	return 0;
}
int proxyclient_discon(struct halfrel* self, struct halfrel* peer)
{
	say("@proxyclient_discon: %.4s\n", &self->foottype);
	return 0;
}
int proxyclient_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@proxyclient_linkup: %.4s\n", &self->foottype);
	return 0;
}
int proxyclient_delete(_obj* art)
{
	return 0;
}
int proxyclient_create(_obj* art, u8* url)
{
	say("@proxyclient_create\n");
	art->vfmt = 0;
	return 0;
}




int proxyserver_read(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int proxyserver_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	say("@proxyserver_write:%p,%p, len=%d\n", art, foot, len);
printmemory(buf, len<16?len:16);

	struct perobj* perobj = (void*)art->priv_256b;
	switch(stack[sp-1].foottype){
	case 'c':	//client
		give_data_into_peer(art,'s', stack,sp, 0,0, buf,len);
		break;
	case 's':	//server
		if(0 == len){			//server ready
			if(perobj->buf){
				//this is http proxy: send cached request directly
				give_data_into_peer(art,'s', stack,sp, 0,0, perobj->buf, perobj->len);
				memorydelete(perobj->buf);
				perobj->buf = 0;
			}
			else{
				//this is https proxy: tell client ready to send request
				give_data_into_peer(art,'c', stack,sp, 0,0, proxy_server0,sizeof(proxy_server0)-1);
			}
		}
		else{
			give_data_into_peer(art,'c', stack,sp, 0,0, buf,len);
		}
		break;
	}
	return 0;
}
int proxyserver_discon(struct halfrel* self, struct halfrel* peer)
{
	say("@proxyserver_discon: %.4s\n", &self->foottype);
	return 0;
}
int proxyserver_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@proxyserver_linkup: %.4s\n", &self->foottype);
	return 0;
}
int proxyserver_delete(_obj* art)
{
	return 0;
}
int proxyserver_create(_obj* art, u8* url)
{
	say("@proxyserver_create\n");
	art->vfmt = 0;
	return 0;
}




int proxymaster_read(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int proxymaster_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	int j;
	u8* ptr;
	struct relation* rel;

	_obj* obj;		//parent
	_obj* Tcp;		//child
	_obj* client;		//client

	_obj* Proxy;		//server
	_obj* socks;

	say("@proxymaster_write: chip=%llx, foot=%.4s, len=%d\n", art, &foot, len);
	printmemory(buf, len<16?len:16);


//0: CONNECT or GET ?
	ptr = 0;
	if(0 == ncmp(buf, "CONNECT ", 8)){
		ptr = buf+8;
	}
	else if( (0 == ncmp(buf, "GET ", 4)) | (0 == ncmp(buf, "POST ", 5)) ){
		for(j=4;j<len-1;j++){
			if( (0xd == buf[j]) && (0xa == buf[j+1]) ){
				if(0 == ncmp(buf+j+2, "Host: ", 6)){
					ptr = buf+j+2+6;
					break;
				}
			}
		}
		if(0 == ptr)return 0;
	}
	else return 0;


//1: create servant
	//parent, child
	obj = stack[sp-2].pchip;
	if(0 == obj)return 0;
	Tcp = obj->sockinfo.child;
	if(0 == Tcp)return 0;

	//master, servant
	Proxy = arterycreate(_Proxy_, 0, 0, 0);
	if(0 == Proxy)return 0;

	//child -> servant
	relationcreate(Proxy, 0, _art_, 'c', Tcp, 0, _sys_, _dst_);


//2: copy host
	struct perobj* artobj = (void*)art->priv_256b;
	struct perobj* perobj = (void*)Proxy->priv_256b;

	for(j=0;j<128;j++){
		if( (ptr[j] == '/') | (ptr[j] <= 0x20) ) {
			perobj->data[j] = 0;
			break;
		}
		perobj->data[j] = ptr[j];
	}
	if(0 != ncmp(buf, "CONNECT ", 8)){
		perobj->len = len;
		perobj->buf = memorycreate(len, 0);

		ptr = perobj->buf;
		for(j=0;j<len;j++)ptr[j] = buf[j];
	}


//3: server prepare
	say("target=%s\n", perobj->data);
	switch(art->hfmt){
		case _socks_:{
			//socksclient -> proxyserver
			socks = arterycreate(_socks_, perobj->data, 0, 0);
			if(0 == socks)break;
			relationcreate(Proxy, 0, _art_, 's', socks, 0, _art_, _dst_);

			//tcpclient -> socksclient
			client = systemcreate(_tcp_, artobj->data, 0, 0);
			if(0 == client)break;
			rel = relationcreate(socks, 0, _art_, _src_, client, 0, _sys_, _dst_);

			//fake ready from tcpclient to socksclient
			relationlinkup((void*)rel->dst, (void*)rel->src);
			break;
		}//socks
		default:{
			//tcpclient -> proxyserver
			client = systemcreate(_tcp_, perobj->data, 0, 0);
			if(0 == client)break;
			rel = relationcreate(Proxy, 0, _art_, 's', client, 0, _sys_, _dst_);

			//fake ready from tcpclient to proxyserver
			stack[sp-2].pchip = client;
			stack[sp-1].pchip = Proxy;
			stack[sp-1].foottype = 's';
			proxyserver_write(Proxy,0, stack,sp, 0,_ok_, 0,0);
			break;
		}//default
	}

	return 0;
}
int proxymaster_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int proxymaster_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@proxymaster_linkup\n");
	return 0;
}
int proxymaster_delete(_obj* art)
{
	return 0;
}
int proxymaster_create(_obj* art, u8* url)
{
	say("@proxymaster.create\n");
	int j,k;
	struct perobj* perobj = (void*)art->priv_256b;
	perobj->buf = 0;
	perobj->len = 0;

	if(0 == url)goto none;
	if(0 == ncmp(url, "socks", 5)){
		for(j=5;j<0x80;j++){
			if(url[j] <= 0x20){
				mysnprintf(perobj->data, 0x80, "127.0.0.1:8888");
				break;
			}
			if(0 == ncmp(url+j, "://", 3)){
				url += j+3;
				for(k=0;k<64;k++){
					if(url[k] <= 0x20){
						perobj->data[k] = 0;
						break;
					}
					perobj->data[k] = url[k];
				}
				break;
			}
		}

		art->hfmt = _socks_;
		return 0;
	}

none:
	art->hfmt = 0;
	say("@proxymaster.ending\n");
	return 0;
}
