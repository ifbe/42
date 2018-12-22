#include "libsoft.h"




int gcodeclient_write(
	struct element* ele, void* sty,
	struct object* obj, void* pin,
	u8* buf, int len)
{
	return 0;
}
int gcodeclient_create(struct element* ele, u8* url)
{
	struct object* obj = systemcreate(_file_, url);
	if(0 == obj)return 0;

	relationcreate(ele, 0, _art_, obj, 0, _fd_);
	return 0;
}





int gcodeserver_write(
	struct element* ele, void* sty,
	struct object* obj, void* pin,
	u8* buf, int len)
{
	return 0;
}
int gcodeserver_create(struct element* ele, u8* url)
{
	struct object* obj = systemcreate(_file_, url);
	if(0 == obj)return 0;

	relationcreate(ele, 0, _art_, obj, 0, _fd_);
	return 0;
}
