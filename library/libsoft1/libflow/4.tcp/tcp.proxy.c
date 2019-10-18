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
        relationwrite(self->pchip, 'a', 0, 0, buf, len);
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

	//side a
	struct object* obj;
	obj = (void*)(peer->chip);
	if(0 == obj)return 0;
	obj = obj->tempobj;
    if(0 == obj)return 0;

    //side b
    void* sys = systemcreate(_tcp_, tmp, 0, 0);
    if(0 == sys)return 0;

    //connect a,b
    void* art = arterycreate(_Proxy_, 0, 0, 0);
    if(0 == art)return 0;
    relationcreate(art, 0, _art_, 'a', obj, 0, _sys_, _dst_);
    relationcreate(art, 0, _art_, 'b', sys, 0, _sys_, _dst_);

    //tell client, ok now
    relationwrite(self->pchip, _src_, arg, idx, proxy_server0, sizeof(proxy_server0)-1);
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
	say("@proxymaster_create\n");
	return 0;
}