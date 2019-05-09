#include "libuser.h"
void* allocarena();
//
int schpin_create(void*, void*);
int schpin_start(void*, void*, void*, void*);
int schpin_read(void*, void*, void*, void*);
//
int schbus_create(void*, void*);
int schbus_start(void*, void*, void*, void*);
int schbus_read(void*, void*, void*, void*);




void scene_import_schematic(struct arena* win, u8* str)
{
	int len;
	u8* buf= win->buf;

	len = openreadclose(str, 0, buf, 0x100000);
	if(len <= 0)return;

	buf[len] = 0;
	say("%d\n%s\n", len, buf);
}




int schnode_leafwrite(struct arena* win, void* wf, void* sc, void* sf, void* buf, int len)
{
	return 0;
}
int schnode_leafread(struct arena* win, void* wf, void* sc, void* sf, void* buf, int len)
{
	return 0;
}
int schnode_rootwrite(struct arena* win, void* wf, void* sc, void* sf, void* buf, int len)
{
	return 0;
}
int schnode_rootread(struct arena* win, void* wf, void* sc, void* sf, void* buf, int len)
{
	return 0;
}




int schnode_stop(struct arena* win, struct style* sty)
{
	return 0;
}
int schnode_start(struct arena* twig, void* tf, struct arena* root, void* rf)
{
	return 0;
}
int schnode_delete(struct arena* win)
{
	return 0;
}
void* schnode_create(u64 type, void* addr)
{
	struct arena* win;

	win = allocarena();
	if(0 == win)return 0;

	win->type = _ctx_;
	win->fmt = _pin_;
	return win;
}
