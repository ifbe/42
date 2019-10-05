#include "libsoft.h"
int termwrite(void*, int);




int hackclient_write(
	struct element* ele, void* sty,
	struct object* obj, void* pin,
	u8* buf, int len)
{
	return 0;
}
int hackclient_read()
{
	return 0;
}
int hackclient_delete(struct element* ele)
{
	return 0;
}
int hackclient_create(struct element* ele, u8* url)
{
	return 0;
}




int hackserver_write(
	struct element* ele, void* sty,
	struct object* obj, void* pin,
	u8* buf, int len)
{
	termwrite(buf, len);
	return 0;
}
int hackserver_read()
{
	return 0;
}
int hackserver_delete(struct element* ele)
{
	return 0;
}
int hackserver_create(struct element* ele, u8* url)
{
	return 0;
}
