#include "libsoft.h"
static u8 proxy_client0[] =
"CONNECT baidu.com:80 HTTP/1.1\r\n"
"\r\n";
static u8 proxy_server0[] =
"HTTP/1.1 200 Connection established\r\n"
"\r\n";




int proxyclient_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int proxyclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	say("@proxyclient_write: %llx, %.4s, %d\n", self->pchip, &self->flag, len);
    printmemory(buf, len<16?len:16);

	return 0;
}
int proxyclient_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int proxyclient_start(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int proxyclient_delete(struct element* ele)
{
	return 0;
}
int proxyclient_create(struct element* ele, u8* url)
{
	say("@proxyclient_create\n");
    ele->stage1 = 0;
	return 0;
}




int proxyserver_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int proxyserver_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	say("@proxyserver_write: chip=%llx, foot=%.4s, len=%d\n", self->chip, &self->flag, len);
    printmemory(buf, len<16?len:16);

    if('a' == self->flag){
        relationwrite(self->pchip, 'b', 0, 0, buf, len);
    }
    if('b' == self->flag){
		if(0 == len){
			//socks success, inform requester
			relationwrite(self->pchip, 'a', 0, 0, proxy_server0, sizeof(proxy_server0)-1);
		}
		else{
			relationwrite(self->pchip, 'a', 0, 0, buf, len);
		}
    }
	return 0;
}
int proxyserver_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int proxyserver_start(struct halfrel* self, struct halfrel* peer)
{
	say("@proxyserver_start\n");
	return 0;
}
int proxyserver_delete(struct element* ele)
{
	return 0;
}
int proxyserver_create(struct element* ele, u8* url)
{
	say("@proxyserver_create\n");
	return 0;
}




int proxymaster_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int proxymaster_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	int j;
	u8 tmp[256];

	struct object* obj;			//parent
	struct object* Tcp;			//child
	struct object* client;		//client

	struct element* ele;		//master
	struct element* Proxy;		//server
	struct element* socks;

	say("@proxymaster_write: chip=%llx, foot=%.4s, len=%d\n", self->chip, &self->flag, len);
    printmemory(buf, len<16?len:16);

	if(0 != ncmp(buf, "CONNECT ", 8))return 0;
	for(j=0;j<len-8;j++){
		if(buf[j+8] <= 0x20){
			tmp[j] = 0;
			break;
		}
		tmp[j] = buf[j+8];
	}
	say("target=%s\n", tmp);

	//parent, child
	obj = (void*)(peer->chip);
	if(0 == obj)return 0;
	Tcp = obj->tempobj;
    if(0 == Tcp)return 0;

	//master, server
	ele = self->pchip;
	if(0 == ele)return 0;
    Proxy = arterycreate(_Proxy_, 0, 0, 0);
    if(0 == Proxy)return 0;

	//child -> server
    relationcreate(Proxy, 0, _art_, 'a', Tcp, 0, _sys_, _dst_);

	//
	switch(ele->name){
	case _socks_:{
		socks = arterycreate(_socks_, 0, 0, 0);
		if(0 == socks)break;
		relationcreate(Proxy, 0, _art_, 'b', socks, 0, _art_, _dst_);

		client = systemcreate(_tcp_, "127.0.0.1:8888", 0, 0);
		if(0 == client)break;
		relationcreate(socks, 0, _art_, _src_, client, 0, _sys_, _dst_);
		break;
	}//socks
	default:{
		client = systemcreate(_tcp_, tmp, 0, 0);
		if(0 == client)break;
		relationcreate(Proxy, 0, _art_, 'b', client, 0, _sys_, _dst_);

		relationwrite(self->pchip, _src_, arg, idx, proxy_server0, sizeof(proxy_server0)-1);
	}//default
	}

	return 0;
}
int proxymaster_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int proxymaster_start(struct halfrel* self, struct halfrel* peer)
{
	say("@proxymaster_start\n");
	return 0;
}
int proxymaster_delete(struct element* ele)
{
	return 0;
}
int proxymaster_create(struct element* ele, u8* url)
{
	say("@proxymaster.start\n");
	if(0 == url)goto none;
	if(0 == ncmp(url, "socks", 5)){
		ele->name = _socks_;
		return 0;
	}

none:
	ele->name = 0;
	say("@proxymaster.ending\n");
	return 0;
}