#include "libsoft.h"




int ahrsclient_write(
	struct element* ele, void* sty,
	struct object* obj, void* pin,
	u8* buf, int len)
{
	printmemory(buf, len);
	return 0;
}
int ahrsclient_create(struct element* ele, u8* url, u8* buf, int len)
{
	return 1;
}
