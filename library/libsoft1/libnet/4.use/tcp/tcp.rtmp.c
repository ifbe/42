#include "libsoft.h"
struct c1s1{
	u32 sendtime;
	u32 mustzero;
	u8 random[0x600-8];
};
struct c2s2{
	u32 copyc1s1;
	u32 recvtime;
	u8 random[0x600-8];
};




int rtmpclient_read(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int rtmpclient_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	switch(art->vfmt){
	}
	art->vfmt += 1;
	return 0;
}
int rtmpclient_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int rtmpclient_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int rtmpclient_delete(_obj* ele)
{
	return 0;
}
int rtmpclient_create(_obj* ele, u8* url)
{
	return 0;
}




int rtmpserver_read(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int rtmpserver_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	printmemory(buf, len);

	switch(art->vfmt){
	}
	return 0;
}
int rtmpserver_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int rtmpserver_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int rtmpserver_delete(_obj* ele)
{
	return 0;
}
int rtmpserver_create(_obj* ele, u8* url)
{
	return 0;
}




int rtmpmaster_read(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int rtmpmaster_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	_obj* obj = stack[sp-2].pchip;
	if(0 == obj)return 0;
	_obj* child = obj->sockinfo.child;
	if(0 == child)return 0;

	_obj* tel = artery_create(_Rtmp_, 0, 0, 0);
	if(0 == tel)return 0;

	relationcreate(tel, 0, _art_, _src_, child, 0, _sys_, _dst_);
	return 0;
}
int rtmpmaster_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int rtmpmaster_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int rtmpmaster_delete(_obj* ele)
{
	return 0;
}
int rtmpmaster_create(_obj* ele, u8* url)
{
	return 0;
}
