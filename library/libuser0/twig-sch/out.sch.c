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
	struct arena* tmp;

	if(_pin_ == type)
	{
		win = allocarena();
		if(win)
		{
			win->type = _twig_;
			win->fmt = _pin_;
			schpin_create(win, addr);
		}
		return win;
	}

	if(_sch_ == type)
	{
		win = addr;

		tmp = schnode_create(_pin_, 0);
		if(tmp)
		{
			relationcreate(tmp, 0, _win_, win, 0, _win_);
			schnode_start(tmp, 0, win, 0);
		}
		return win;
	}

	return 0;
}