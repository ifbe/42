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



int telnetclient_write(
	struct element* ele, void* sty,
	struct object* obj, void* pin,
	u8* buf, int len)
{
	if(ele->stage1 >= 6)
	{
		say("%.*s", len, buf);
		return 0;
	}

	switch(ele->stage1)
	{
		case 0:relationwrite(ele, _src_, 0, 0, cpacket0, sizeof(cpacket0));break;
		case 1:relationwrite(ele, _src_, 0, 0, cpacket1, sizeof(cpacket1));break;
		case 2:relationwrite(ele, _src_, 0, 0, cpacket2, sizeof(cpacket2));break;
		case 3:relationwrite(ele, _src_, 0, 0, cpacket3, sizeof(cpacket3));break;
		case 4:relationwrite(ele, _src_, 0, 0, cpacket4, sizeof(cpacket4));break;
		case 5:relationwrite(ele, _src_, 0, 0, cpacket5, sizeof(cpacket5));break;
	}
	ele->stage1 += 1;
	return 0;
}
int telnetclient_read()
{
	return 0;
}
int telnetclient_delete(struct element* ele)
{
	return 0;
}
int telnetclient_create(struct element* ele, u8* url)
{
	int ret;
	void* obj = systemcreate(_tcp_, url);
	if(0 == obj)return 0;

	relationcreate(ele, 0, _art_, 0, obj, 0, _sys_, 0);

	ele->stage1 = 0;
	telnetclient_write(ele, 0, obj, 0, 0, 0);
	return 0;
}




int telnetserver_write(
	struct element* ele, void* sty,
	struct object* obj, void* pin,
	u8* buf, int len)
{
	printmemory(buf, len);

	switch(ele->stage1)
	{
		case 0:relationwrite(ele, _src_, 0, 0, spacket0, sizeof(spacket0));break;
		case 1:relationwrite(ele, _src_, 0, 0, spacket1, sizeof(spacket1));break;
		case 2:relationwrite(ele, _src_, 0, 0, spacket2, sizeof(spacket2));break;
		case 3:relationwrite(ele, _src_, 0, 0, spacket3, sizeof(spacket3));break;
		case 4:relationwrite(ele, _src_, 0, 0, spacket4, sizeof(spacket4));break;
		case 5:relationwrite(ele, _src_, 0, 0, spacket5, sizeof(spacket5));break;
	}
	ele->stage1 += 1;
	return 0;
}
int telnetserver_read()
{
	return 0;
}
int telnetserver_delete(struct element* ele)
{
	return 0;
}
int telnetserver_create(struct element* ele, u8* url)
{
	return 0;
}




int telnetmaster_write(
	struct element* ele, void* sty,
	struct object* obj, void* pin,
	u8* buf, int len)
{
	struct element* e = arterycreate(_Telnet_, 0);
	if(e)relationcreate(e, 0, _art_, 0, obj, 0, _sys_, 0);

	e->stage1 = 0;
	telnetserver_write(e, 0, obj, 0, buf, len);
	return 0;
}
int telnetmaster_read()
{
	return 0;
}
int telnetmaster_delete(struct element* ele)
{
	return 0;
}
int telnetmaster_create(struct element* ele, u8* url)
{
	int ret;
	void* obj = systemcreate(_TCP_, url);
	if(0 == obj)return 0;

	relationcreate(ele, 0, _art_, 0, obj, 0, _sys_, 0);
	return 0;
}
