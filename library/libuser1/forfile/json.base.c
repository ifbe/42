#include "libuser.h"




int jsonnode_write(struct actor* win, void* wf, void* sc, void* sf, void* buf, int len)
{
	return 0;
}
int jsonnode_read(struct actor* win, void* wf, void* sc, void* sf, void* buf, int len)
{
	return 0;
}
int jsonnode_delete(struct actor* win)
{
	return 0;
}
int jsonnode_create(struct actor* win, void* str)
{
	win->len = 0;
	win->buf = memorycreate(0x100000, 0);
	return 0;
}
