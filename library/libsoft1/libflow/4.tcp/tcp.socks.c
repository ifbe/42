#include "libsoft.h"
#define _ok_ hex16('o','k')
int decstr2data(void*, void*);




struct socks5_request{
    u8 ver;     //5
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




int socksclient_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int socksclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
    int j,port;
    u8 tmp[256];
    struct element* ele;
    struct socks5_request* req;
	say("@socksclient_write: %llx, %.4s, %x\n", self->pchip, &self->flag, len);
    if(len>0)printmemory(buf, len<16?len:16);

    ele = self->pchip;
    switch(self->flag){
    case _dst_:{
        //dst to src
        relationwrite(ele, _src_, 0, 0, buf, len);
        break;
    }//dst
    case _src_:{
        if(3 == ele->stage1){
            //src to dst
            relationwrite(ele, _dst_, 0, 0, buf, len);
            return 0;
        }

        if(2 == ele->stage1){
            //recv: socks5 reply1
            //send: socks5 ready
            if(0){
                req = (void*)(ele->data);
                j = req->len;
                port = (req->url[j]<<8)+req->url[j+1];
                j = mysnprintf(tmp, 256, httpreq, j, req->url, port);

                printmemory(tmp, j);
                relationwrite(ele, _src_, 0, 0, tmp, j);
            }
            else{
                relationwrite(ele, _dst_, 0, _ok_, 0, 0);
            }
            ele->stage1 = 3;
            return 0;
        }

        if(1 == ele->stage1){
            //recv: socks5 reply0
            //send: socks5 packet1(request)
            req = (void*)(ele->data);
 
            printmemory(req, 7+req->len);
            relationwrite(ele, _src_, 0, 0, req, 7+req->len);
 
            ele->stage1 = 2;
            return 0;
        }//socks5 

        if(0 == ele->stage1){
            //recv: socket ready
            //send: client packet0(hello)
            relationwrite(ele, _src_, 0, 0, socks5_client0, 3);
            ele->stage1 = 1;
            return 0;
        }
    }//src
    }//switch
	return 0;
}
int socksclient_stop(struct halfrel* self, struct halfrel* peer)
{
	say("@socksclient_stop: %.4s\n", &self->flag);
	return 0;
}
int socksclient_start(struct halfrel* self, struct halfrel* peer)
{
    struct element* ele;
    struct object* obj;
	say("@socksclient_start: %.4s\n", &self->flag);

    if(_src_ == self->flag){
		obj = peer->pchip;
        if(_sys_ == obj->tier)socksclient_write(self, peer, 0, _ok_, 0, 0);
    }
    return 0;
}
int socksclient_delete(struct element* ele)
{
	return 0;
}
int socksclient_create(struct element* ele, char* url)
{
    int j,port;
    struct socks5_request* req;
	say("@socksclient_create: %llx\n", url);
    ele->stage1 = 0;

    if(0 == url)url = "www.baidu.com";
    say("%s\n", url);

    //head
    req = (void*)(ele->data);
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




int socksserver_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int socksserver_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	say("@socksserver_write: chip=%llx, foot=%.4s, len=%d\n", self->chip, &self->flag, len);
    printmemory(buf, len<16?len:16);

    if('a' == self->flag){
        relationwrite(self->pchip, 'b', 0, 0, buf, len);
    }
    if('b' == self->flag){
        relationwrite(self->pchip, 'a', 0, 0, buf, len);
    }
	return 0;
}
int socksserver_stop(struct halfrel* self, struct halfrel* peer)
{
	say("@socksserver_stop: %.4s\n", &self->flag);
	return 0;
}
int socksserver_start(struct halfrel* self, struct halfrel* peer)
{
	say("@socksserver_start: %.4s\n", &self->flag);
	return 0;
}
int socksserver_delete(struct element* ele)
{
	return 0;
}
int socksserver_create(struct element* ele, u8* url)
{
	say("@socksserver_create\n");
	return 0;
}




int socksmaster_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int socksmaster_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
    int j;
    u8* ch;
    struct socks5_request* req;
	say("@socksmaster_write: chip=%llx, foot=%.4s, len=%d\n", self->chip, &self->flag, len);
    printmemory(buf, len<16?len:16);

    if(len < 3)return 0;
    if(3 == len){
        ch = buf;
        if( (5 == ch[0]) && (1 == ch[1]) && (0 == ch[2]) ){
            relationwrite(self->pchip, _src_, arg, idx, socks5_server0, 2);
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
	struct object* obj;
	obj = (void*)(peer->chip);
	if(0 == obj)return 0;
	obj = obj->tempobj;
    if(0 == obj)return 0;
say("2\n");
    //side b
    void* sys = systemcreate(_tcp_, tmp, 0, 0);
    if(0 == sys)return 0;
say("3\n");
    //connect a,b
    void* art = arterycreate(_Socks_, 0, 0, 0);
    if(0 == art)return 0;
    relationcreate(art, 0, _art_, 'a', obj, 0, _sys_, _dst_);
    relationcreate(art, 0, _art_, 'b', sys, 0, _sys_, _dst_);
say("4\n");
    //tell client, ok now
    relationwrite(self->pchip, _src_, arg, idx, socks5_server1, 10);
	return 0;
}
int socksmaster_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int socksmaster_start(struct halfrel* self, struct halfrel* peer)
{
	say("@socksmaster_start\n");
	return 0;
}
int socksmaster_delete(struct element* ele)
{
	return 0;
}
int socksmaster_create(struct element* ele, u8* url)
{
	say("@socksmaster_create\n");
	return 0;
}