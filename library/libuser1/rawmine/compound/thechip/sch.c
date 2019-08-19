#include "libuser.h"
void* allocactor();
//
int schpin_create(void*, void*);
int schpin_start(void*, void*, void*, void*);
int schpin_read(void*, void*, void*, void*);
//
int schbus_create(void*, void*);
int schbus_start(void*, void*, void*, void*);
int schbus_read(void*, void*, void*, void*);




void scene_import_schematic(struct actor* win, u8* str)
{
	int len;
	u8* buf= win->buf;

	len = openreadclose(str, 0, buf, 0x100000);
	if(len <= 0)return;

	buf[len] = 0;
	say("%d\n%s\n", len, buf);
}




int schnode_leafwrite(struct actor* win, void* wf, void* sc, void* sf, void* buf, int len)
{
	return 0;
}
int schnode_leafread(struct actor* win, void* wf, void* sc, void* sf, void* buf, int len)
{
	return 0;
}
int schnode_rootwrite(struct actor* win, void* wf, void* sc, void* sf, void* buf, int len)
{
	return 0;
}
int schnode_rootread(struct actor* win, void* wf, void* sc, void* sf, void* buf, int len)
{
	return 0;
}




int schnode_stop(struct actor* win, struct style* sty)
{
	return 0;
}
int schnode_start(struct actor* twig, void* tf, struct actor* root, void* rf)
{
	return 0;
}
int schnode_delete(struct actor* win)
{
	return 0;
}
void* schnode_create(u64 type, void* addr)
{
	struct actor* win;

	win = allocactor();
	if(0 == win)return 0;

	win->type = _ctx_;
	win->fmt = _pin_;
	return win;
}
