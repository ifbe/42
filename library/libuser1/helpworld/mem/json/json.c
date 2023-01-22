#include "libuser.h"




int jsonnode_write(_obj* win, void* wf, void* sc, void* sf, void* buf, int len)
{
	return 0;
}
int jsonnode_read(_obj* win, void* wf, void* sc, void* sf, void* buf, int len)
{
	return 0;
}
int jsonnode_delete(_obj* win)
{
	return 0;
}
int jsonnode_create(_obj* win, void* str)
{
	win->json.len = 0;
	win->json.buf = memorycreate(0x100000, 0);
	return 0;
}
