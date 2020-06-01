#include "libsoft.h"
static u8 cpacket0[] = {
	0xff, 0xfd, 0x03,
	0xff, 0xfb, 0x18,
	0xff, 0xfb, 0x1f,
	0xff, 0xfb, 0x20,
	0xff, 0xfb, 0x21,
	0xff, 0xfb, 0x22,
	0xff, 0xfb, 0x27,
	0xff, 0xfd, 0x5
};
static u8 spacket0[] = {
	0xff, 0xfd, 0x18,
	0xff, 0xfd, 0x20,
	0xff, 0xfd, 0x23,
	0xff, 0xfd, 0x27
};
static u8 cpacket1[] = {
	0xff, 0xfc, 0x23
};
static u8 spacket1[] = {
	0xff, 0xfb, 0x03,
	0xff, 0xfd, 0x1f,
	0xff, 0xfd, 0x21,
	0xff, 0xfe, 0x22,
	0xff, 0xfb, 0x05
};
static u8 cpacket2[] = {
	0xff, 0xfa, 0x1f, 0x00, 0x78, 0x00, 0x1e,
	0xff, 0xf0
};
static u8 spacket2[] = {
	0xff, 0xfa, 0x20, 0x01,
	0xff, 0xf0,
	0xff, 0xfa, 0x27, 0x01,
	0xff, 0xf0,
	0xff, 0xfa, 0x18, 0x01,
	0xff, 0xf0
};
static u8 cpacket3[] = {
	0xff, 0xfa, 0x20, 0x00, 0x33, 0x38, 0x34, 0x30, 0x30, 0x2c, 0x33, 0x38, 0x34, 0x30, 0x30,
	0xff, 0xf0,
	0xff, 0xfa, 0x27, 0x00,
	0xff, 0xf0,
	0xff, 0xfa, 0x18, 0x00, 0x78, 0x74, 0x65, 0x72, 0x6d, 0x2d, 0x32, 0x35, 0x36, 0x63, 0x6f, 0x6c, 0x6f, 0x72,
	0xff, 0xf0
};
static u8 spacket3[] = {
	0xff, 0xfd, 0x01
};
static u8 cpacket4[] = {
	0xff, 0xfc, 0x01
};
static u8 spacket4[] = {
	0xff, 0xfb, 0x01
};
static u8 cpacket5[] = {
	0xff, 0xfd, 0x01
};
static u8 spacket5[] = {
	'o','k',0xd,0xa
};




int telnetclient_read(_art* art,int foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int telnetclient_write(_art* art,int foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	switch(art->stage1)
	{
		case 1:give_data_into_peer(art,_src_, stack,sp, 0,0, cpacket1,sizeof(cpacket1));break;
		case 2:give_data_into_peer(art,_src_, stack,sp, 0,0, cpacket2,sizeof(cpacket2));break;
		case 3:give_data_into_peer(art,_src_, stack,sp, 0,0, cpacket3,sizeof(cpacket3));break;
		case 4:give_data_into_peer(art,_src_, stack,sp, 0,0, cpacket4,sizeof(cpacket4));break;
		case 5:give_data_into_peer(art,_src_, stack,sp, 0,0, cpacket5,sizeof(cpacket5));break;
		default:{
			say("%.*s", len, buf);
			return 0;
		}
	}
	art->stage1 += 1;
	return 0;
}
int telnetclient_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int telnetclient_linkup(struct halfrel* self, struct halfrel* peer)
{
	struct artery* ele = self->pchip;
	give_data_into_peer(ele,_src_, 0,0, 0,0, cpacket0,sizeof(cpacket0));

	ele->stage1 = 1;
	return 0;
}
int telnetclient_delete(struct artery* ele)
{
	return 0;
}
int telnetclient_create(struct artery* ele, u8* url)
{
	ele->stage1 = 0;
	return 0;
}




int telnetserver_read(_art* art,int foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int telnetserver_write(_art* art,int foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	printmemory(buf, len);

	switch(art->stage1)
	{
		case 0:give_data_into_peer(art,_src_, stack,sp, 0,0, spacket0,sizeof(spacket0));break;
		case 1:give_data_into_peer(art,_src_, stack,sp, 0,0, spacket1,sizeof(spacket1));break;
		case 2:give_data_into_peer(art,_src_, stack,sp, 0,0, spacket2,sizeof(spacket2));break;
		case 3:give_data_into_peer(art,_src_, stack,sp, 0,0, spacket3,sizeof(spacket3));break;
		case 4:give_data_into_peer(art,_src_, stack,sp, 0,0, spacket4,sizeof(spacket4));break;
		case 5:give_data_into_peer(art,_src_, stack,sp, 0,0, spacket5,sizeof(spacket5));break;
	}
	art->stage1 += 1;
	return 0;
}
int telnetserver_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int telnetserver_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int telnetserver_delete(struct artery* ele)
{
	return 0;
}
int telnetserver_create(struct artery* ele, u8* url)
{
	return 0;
}




int telnetmaster_read(_art* art,int foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int telnetmaster_write(_art* art,int foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	struct sysobj* obj = stack[sp-2].pchip;
	if(0 == obj)return 0;
	obj = obj->tempobj;
	if(0 == obj)return 0;

	struct artery* tel = arterycreate(_Telnet_, 0, 0, 0);
	if(0 == tel)return 0;

	relationcreate(tel, 0, _art_, _src_, obj, 0, _sys_, _dst_);
	stack[sp-2].pchip = obj;
	stack[sp-1].pchip = tel;
	telnetserver_write(tel,_src_, stack,sp, arg,idx, buf,len);
	return 0;
}
int telnetmaster_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int telnetmaster_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int telnetmaster_delete(struct artery* ele)
{
	return 0;
}
int telnetmaster_create(struct artery* ele, u8* url)
{
	return 0;
}
