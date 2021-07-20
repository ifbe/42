#include "libsoft.h"
#define _ok_ hex16('o','k')
int decstr2data(void*, void*);




struct socks5_request{
	u8 ver;	 //5
	u8 cmd;
		//CONNECT X'01'
		//BIND X'02'
		//UDP ASSOCIATE X'03'
	u8 rsv;
	u8 atyp;
		//IP V4 address: X'01'
		//DOMAINNAME: X'03'
		//IP V6 address: X'04'

	u8 len;
	u8 url[0];
		//DST.ADDR: desired destination address
		//DST.PORT: desired destination port in network octet order
};
static u8 socks5_client0[] = {5,1,0};
static u8 socks5_server0[] = {5, 0};
static u8 socks5_client1[] = {0};
static u8 socks5_server1[] = {5, 0, 0, 1, 0, 0, 0, 0, 0, 0};
static u8 httpreq[] =
"GET / HTTP/1.1\r\n"
"Host: %.*s:%d\r\n"
"\r\n";




int socksclient_read(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int socksclient_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	int j,port;
	u8 tmp[256];
	struct socks5_request* req;
	say("@socksclient_write: %llx, %.4s, %x\n", art, &foot, len);
	if(len>0)printmemory(buf, len<16?len:16);

	switch(stack[sp-1].flag){
	case _dst_:{
		//dst to src
		give_data_into_peer(art,_src_, stack,sp, 0,0, buf, len);
		break;
	}//dst
	case _src_:{
		if(3 == art->vfmt){
			//src to dst
			give_data_into_peer(art,_dst_, stack,sp, 0,0, buf,len);
			return 0;
		}

		if(2 == art->vfmt){
			//recv: socks5 reply1
			//send: socks5 ready
			if(0){
				req = (void*)(art->priv_256b);
				j = req->len;
				port = (req->url[j]<<8)+req->url[j+1];
				j = mysnprintf(tmp, 256, httpreq, j, req->url, port);

				printmemory(tmp, j);
				give_data_into_peer(art,_src_, stack,sp, 0,0, tmp,j);
			}
			else{
				give_data_into_peer(art,_dst_, stack,sp, 0,_ok_, 0,0);
			}
			art->vfmt = 3;
			return 0;
		}

		if(1 == art->vfmt){
			//recv: socks5 reply0
			//send: socks5 packet1(request)
			req = (void*)(art->priv_256b);
 
			printmemory(req, 7+req->len);
			give_data_into_peer(art,_src_, stack,sp, 0,0, req,7+req->len);
 
			art->vfmt = 2;
			return 0;
		}//socks5 
	}//src
	}//switch
	return 0;
}
int socksclient_discon(struct halfrel* self, struct halfrel* peer)
{
	say("@socksclient_discon: %.4s\n", &self->flag);
	return 0;
}
int socksclient_linkup(struct halfrel* self, struct halfrel* peer)
{
	_obj* ele;
	say("@socksclient_linkup: %.4s\n", &self->flag);

	if(_src_ == self->flag){
		ele = self->pchip;
		give_data_into_peer(ele, _src_, 0,0, 0,0, socks5_client0,3);
		ele->vfmt = 1;
	}
	return 0;
}
int socksclient_delete(_obj* ele)
{
	return 0;
}
int socksclient_create(_obj* ele, char* url)
{
	int j,port;
	struct socks5_request* req;
	say("@socksclient_create: %llx\n", url);
	ele->vfmt = 0;

	if(0 == url)url = "www.baidu.com";
	say("%s\n", url);

	//head
	req = (void*)(ele->priv_256b);
	req->ver = 5;
	req->cmd = 1;
	req->rsv = 0;
	req->atyp = 3;

	//addr
	for(j=0;j<128;j++){
		if(url[j] <= 0x20)break;
		if(url[j] == ':')break;
		req->url[j] = url[j];
	}
	req->len = j;

	//port
	if(url[j] != ':')port = 80;
	else{
		decstr2data(url+j+1, &port);
	}
	req->url[req->len+0] = (port>>8)&0xff;
	req->url[req->len+1] = port&0xff;
	return 0;
}




int socksserver_read(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int socksserver_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	say("@socksserver_write:%p,%p, len=%d\n", art, foot, len);
	printmemory(buf, len<16?len:16);

	switch(stack[sp-1].flag){
	case 'a':
		give_data_into_peer(art,'b', stack,sp, 0,0, buf,len);
		break;
	case 'b':
		give_data_into_peer(art,'a', stack,sp, 0,0, buf,len);
		break;
	}
	return 0;
}
int socksserver_discon(struct halfrel* self, struct halfrel* peer)
{
	say("@socksserver_discon: %.4s\n", &self->flag);
	return 0;
}
int socksserver_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@socksserver_linkup: %.4s\n", &self->flag);
	return 0;
}
int socksserver_delete(_obj* ele)
{
	return 0;
}
int socksserver_create(_obj* ele, u8* url)
{
	say("@socksserver_create\n");
	return 0;
}




int socksmaster_read(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int socksmaster_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, void* buf, int len)
{
	int j;
	u8* ch;
	struct socks5_request* req;
	say("@socksmaster_write: chip=%llx, foot=%.4s, len=%d\n", art, &foot, len);
	printmemory(buf, len<16?len:16);

	if(len < 3)return 0;
	if(3 == len){
		ch = buf;
		if( (5 == ch[0]) && (1 == ch[1]) && (0 == ch[2]) ){
			give_data_into_peer(art,_src_, stack,sp, arg,idx, socks5_server0,2);
		}
		return 0;
	}

	req = buf;
	if(5 != req->ver){
		say("\n????????\n");
		return 0;
	}

	u8 tmp[256];
	ch = req->url;
	j = req->len;
	mysnprintf(tmp, 256, "%.*s:%d", j, ch, (ch[j]<<8) + ch[j+1]);
	say("ver=%x,cmd=%x,atyp=%x: %s\n", req->ver, req->cmd, req->atyp, tmp);
	if(3 != req->atyp)return 0;
say("1\n");
	//side a
	_obj* obj = stack[sp-1].pchip;
	if(0 == obj)return 0;
	_obj* child = obj->sockinfo.child;
	if(0 == child)return 0;
say("2\n");
	//side b
	void* sys = systemcreate(_tcp_, tmp, 0, 0);
	if(0 == sys)return 0;
say("3\n");
	//connect s->a s->b
	void* s5 = arterycreate(_Socks_, 0, 0, 0);
	if(0 == s5)return 0;
	relationcreate(s5, 0, _art_, 'a', child, 0, _sys_, _dst_);
	relationcreate(s5, 0, _art_, 'b', sys, 0, _sys_, _dst_);
say("4\n");
	//tell client, ok now
	give_data_into_peer(art,_src_, stack,sp, arg,idx, socks5_server1,10);
	return 0;
}
int socksmaster_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int socksmaster_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@socksmaster_linkup\n");
	return 0;
}
int socksmaster_delete(_obj* ele)
{
	return 0;
}
int socksmaster_create(_obj* ele, u8* url)
{
	say("@socksmaster_create\n");
	return 0;
}
