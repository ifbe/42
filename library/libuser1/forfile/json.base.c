#include "libuser.h"




int jsonnode_write(struct entity* win, void* wf, void* sc, void* sf, void* buf, int len)
{
	return 0;
}
int jsonnode_read(struct entity* win, void* wf, void* sc, void* sf, void* buf, int len)
{
	return 0;
}
int jsonnode_delete(struct entity* win)
{
	return 0;
}
int jsonnode_create(struct entity* win, void* str)
{
	win->jsonlen = 0;
	win->jsonbuf = memorycreate(0x100000, 0);
	return 0;
}
