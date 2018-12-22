#include "libsoft.h"




int serveclient_write(
	struct element* ele, void* sty,
	struct object* obj, void* pin,
	u8* buf, int len)
{
	return 0;
}
int serveclient_read()
{
	return 0;
}
int serveclient_delete(struct element* ele)
{
	return 0;
}
int serveclient_create(struct element* ele, u8* url, u8* buf, int len)
{
	int ret;
	void* obj = systemcreate(_tcp_, url);
	if(0 == obj)return 0;

	relationcreate(ele, 0, _art_, obj, 0, _fd_);
	return 1;
}




int serveserver_leafwrite(
	struct element* ele, void* sty,
	struct object* sc, void* sf,
	u8* buf, int len)
{
	return 0;
}
int serveserver_leafread(
	struct element* ele, void* sty,
	struct object* obj, void* pin,
	u8* buf, int len)
{
	return 0;
}
int serveserver_rootwrite(
	struct element* ele, void* sty,
	struct object* obj, void* pin,
	u8* buf, int len)
{
	printmemory(buf,len);
	return 0;
}
int serveserver_rootread(
	struct element* ele, void* sty,
	struct object* obj, void* pin,
	u8* buf, int len)
{
	return 0;
}
int serveserver_delete(struct element* ele)
{
	return 0;
}
int serveserver_create(struct element* ele, u8* url)
{
	return 0;
}




int servemaster_write(
	struct element* ele, void* sty,
	struct object* obj, void* pin,
	u8* buf, int len)
{
	return 0;
}
int servemaster_read()
{
	return 0;
}
int servemaster_delete(struct element* ele)
{
	return 0;
}
int servemaster_create(struct element* ele, u8* url)
{
	int ret;
	void* obj = systemcreate(_TCP_, url);
	if(0 == obj)return 0;

	relationcreate(ele, 0, _art_, obj, 0, _fd_);
	return 0;
}