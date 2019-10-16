#include "libsoft.h"
static u8 socks5_reply[] = {5, 0};
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




int socksclient_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int socksclient_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	say("@socksclient_write\n");
    printmemory(buf, len);
	return 0;
}
int socksclient_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int socksclient_start(struct halfrel* self, struct halfrel* peer)
{
	say("@socksclient_start\n");
	return 0;
}
int socksclient_delete(struct element* ele)
{
	return 0;
}
int socksclient_create(struct element* ele, u8* url)
{
	say("@socksclient_create\n");
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
	say("@socksmaster_write\n");
    printmemory(buf, len);

    if(3 == len){
        ch = buf;
        if( (5 == ch[0]) && (1 == ch[1]) && (0 == ch[2]) ){
            relationwrite(self->pchip, _src_, arg, idx, socks5_reply, 2);
        }
        return 0;
    }
    if(len > 3){
        req = buf;
        ch = req->url;
        j = req->len;
        say("ver=%x,cmd=%x,atyp=%x: %.*s:%d\n",
            req->ver, req->cmd, req->atyp,
            j, ch, (ch[j]<<8) + ch[j+1]
        );
    }
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