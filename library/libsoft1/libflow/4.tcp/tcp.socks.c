#include "libsoft.h"
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
static u8 socks5_client2[] =
"GET / HTTP/1.1\r\n"
"Host: www.baidu.com:80\r\n"
"\r\n";




int socksclient_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int socksclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
    struct element* ele;
    struct socks5_request req;
	say("@socksclient_write\n");
    printmemory(buf, len);

    ele = self->pchip;
    if(2 == ele->stage1){
        relationwrite(ele, _src_, 0, 0, socks5_client2, sizeof(socks5_client2));
        ele->stage1 = 3;
        return 0;
    }
    if(1 == ele->stage1){
        req.ver = 5;
        req.cmd = 1;
        req.rsv = 0;
        req.atyp = 3;
        req.len = snprintf(req.url, 32, "www.baidu.com");
        req.url[req.len+0] = 0;
        req.url[req.len+1] = 80;

        relationwrite(ele, _src_, 0, 0, &req, 7+req.len);
        ele->stage1 = 2;
        return 0;
    }
	return 0;
}
int socksclient_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int socksclient_start(struct halfrel* self, struct halfrel* peer)
{
    struct element* ele;
	say("@socksclient_start: %.4s\n", &self->flag);

    ele = self->pchip;
    relationwrite(ele, _src_, 0, 0, socks5_client0, 3);
    ele->stage1 = 1;
	return 0;
}
int socksclient_delete(struct element* ele)
{
	return 0;
}
int socksclient_create(struct element* ele, u8* url)
{
	say("@socksclient_create\n");
    ele->stage1 = 0;
	return 0;
}




int socksserver_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int socksserver_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	say("@socksserver_write: chip=%llx, foot=%.4s, len=%d\n", self->chip, &self->flag, len);
    //printmemory(buf, len);

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
	return 0;
}
int socksserver_start(struct halfrel* self, struct halfrel* peer)
{
	say("@socksserver_start\n");
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
    if(len < 16)printmemory(buf, len);

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
    if(0 == sys)return 0;
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