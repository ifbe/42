#include "libsoft.h"
#define _ok_ hex16('o','k')




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
int proxyclient_discon(struct halfrel* self, struct halfrel* peer)
{
	say("@proxyclient_discon: %.4s\n", &self->flag);
	return 0;
}
int proxyclient_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@proxyclient_linkup: %.4s\n", &self->flag);
	return 0;
}
int proxyclient_delete(struct artery* ele)
{
	return 0;
}
int proxyclient_create(struct artery* ele, u8* url)
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
	struct artery* ele;
	say("@proxyserver_write: chip=%llx, foot=%.4s, len=%d\n", self->chip, &self->flag, len);
printmemory(buf, len<16?len:16);

	ele = self->pchip;
	if('c' == self->flag){		//client
		relationwrite(ele, 's', 0, 0, buf, len);
	}
	if('s' == self->flag){		//server
		if(0 == len){			//server ready
			if(ele->buf0){
				//this is http proxy: send cached request directly
				relationwrite(ele, 's', 0, 0, ele->buf0, ele->len);
				memorydelete(ele->buf0);
				ele->buf0 = 0;
			}
			else{
				//this is https proxy: tell client ready to send request
				relationwrite(ele, 'c', 0, 0, proxy_server0, sizeof(proxy_server0)-1);
			}
		}
		else{
			relationwrite(ele, 'c', 0, 0, buf, len);
		}
	}
	return 0;
}
int proxyserver_discon(struct halfrel* self, struct halfrel* peer)
{
	say("@proxyserver_discon: %.4s\n", &self->flag);
	return 0;
}
int proxyserver_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@proxyserver_linkup: %.4s\n", &self->flag);
	return 0;
}
int proxyserver_delete(struct artery* ele)
{
	return 0;
}
int proxyserver_create(struct artery* ele, u8* url)
{
	say("@proxyserver_create\n");
	ele->stage1 = 0;
	return 0;
}




int proxymaster_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int proxymaster_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	int j;
	u8* ptr;
	struct relation* rel;

	struct sysobj* obj;			//parent
	struct sysobj* Tcp;			//child
	struct sysobj* client;		//client

	struct artery* ele;		//master
	struct artery* Proxy;		//server
	struct artery* socks;

	say("@proxymaster_write: chip=%llx, foot=%.4s, len=%d\n", self->chip, &self->flag, len);
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
	obj = (void*)(peer->chip);
	if(0 == obj)return 0;
	Tcp = obj->tempobj;
	if(0 == Tcp)return 0;

	//master, servant
	ele = self->pchip;
	if(0 == ele)return 0;
	Proxy = arterycreate(_Proxy_, 0, 0, 0);
	if(0 == Proxy)return 0;

	//child -> servant
	relationcreate(Proxy, 0, _art_, 'c', Tcp, 0, _sys_, _dst_);


//2: copy host
	for(j=0;j<128;j++){
		if( (ptr[j] == '/') | (ptr[j] <= 0x20) ) {
			Proxy->data[j] = 0;
			break;
		}
		Proxy->data[j] = ptr[j];
	}
	if(0 != ncmp(buf, "CONNECT ", 8)){
		Proxy->len = len;
		Proxy->buf0 = memorycreate(len, 0);

		ptr = Proxy->buf0;
		for(j=0;j<len;j++)ptr[j] = buf[j];
	}


//3: server prepare
	say("target=%s\n", Proxy->data);
	switch(ele->name){
		case _socks_:{
			//socksclient -> proxyserver
			socks = arterycreate(_socks_, Proxy->data, 0, 0);
			if(0 == socks)break;
			relationcreate(Proxy, 0, _art_, 's', socks, 0, _art_, _dst_);

			//tcpclient -> socksclient
			client = systemcreate(_tcp_, ele->data, 0, 0);
			if(0 == client)break;
			rel = relationcreate(socks, 0, _art_, _src_, client, 0, _sys_, _dst_);

			//fake ready from tcpclient to socksclient
			self = (void*)&rel->dstchip;
			peer = (void*)&rel->srcchip;
			relationlinkup(self, peer);
			break;
		}//socks
		default:{
			//tcpclient -> proxyserver
			client = systemcreate(_tcp_, Proxy->data, 0, 0);
			if(0 == client)break;
			rel = relationcreate(Proxy, 0, _art_, 's', client, 0, _sys_, _dst_);

			//fake ready from tcpclient to proxyserver
			self = (void*)&rel->dstchip;
			peer = (void*)&rel->srcchip;
			proxyserver_write(self, peer, 0, _ok_, 0, 0);
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
int proxymaster_delete(struct artery* ele)
{
	return 0;
}
int proxymaster_create(struct artery* ele, u8* url)
{
	int j,k;
	say("@proxymaster.create\n");

	if(0 == url)goto none;
	if(0 == ncmp(url, "socks", 5)){
		for(j=5;j<0x80;j++){
			if(url[j] <= 0x20){
				mysnprintf(ele->data, 0x80, "127.0.0.1:8888");
				break;
			}
			if(0 == ncmp(url+j, "://", 3)){
				url += j+3;
				for(k=0;k<64;k++){
					if(url[k] <= 0x20){
						ele->data[k] = 0;
						break;
					}
					ele->data[k] = url[k];
				}
				break;
			}
		}

		ele->name = _socks_;
		return 0;
	}

none:
	ele->name = 0;
	say("@proxymaster.ending\n");
	return 0;
}
