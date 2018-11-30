#include "libuser.h"




int jsonnode_leafwrite(struct arena* win, void* wf, void* sc, void* sf, void* buf, int len)
{
	return 0;
}
int jsonnode_leafread(struct arena* win)
{
	return 0;
}
int jsonnode_rootwrite(struct arena* win, void* wf, void* sc, void* sf, void* buf, int len)
{
	void* dc;
	void* df;
	struct relation* orel = win->orel0;

	if(0 == orel)
	{
		say("@jsonnode_write: %.*s\n", len, buf);
		artery_leafwrite(sc, sf, win, wf, "OK", 2);
		return 0;
	}

	while(1)
	{
		if(0 == orel)break;

		dc = (void*)(orel->dstchip);
		df = (void*)(orel->dstfoot);
		if(_act_ == orel->dsttype)
		{
			actor_rootwrite(dc, df, win, 0, buf, len);
		}

		orel = samesrcnextdst(orel);
	}

	actorread_all(win);
	artery_leafwrite(sc, sf, win, wf, win->buf, win->len);
	return 0;
}
int jsonnode_rootread(struct arena* win, void* wf, void* sc, void* sf, void* buf, int len)
{
	return 0;
}
int jsonnode_delete(struct arena* win)
{
	return 0;
}
int jsonnode_create(struct arena* win, void* str)
{
	win->len = 0;
	win->buf = memorycreate(0x100000);
	return 0;
}