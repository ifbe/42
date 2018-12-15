#include "libuser.h"
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
int schnode_delete(struct arena* win)
{
	return 0;
}
int schnode_create(struct arena* win, void* str)
{
	win->buf = memorycreate(1000*1000);
	if(str)scene_import_schematic(win, str);
	return 0;
}