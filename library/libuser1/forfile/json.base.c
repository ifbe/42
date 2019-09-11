#include "libuser.h"




int jsonnode_leafwrite(struct actor* win, void* wf, void* sc, void* sf, void* buf, int len)
{
	return 0;
}
int jsonnode_leafread(struct actor* win)
{
	return 0;
}
int jsonnode_rootwrite(struct actor* win, void* wf, void* sc, void* sf, void* buf, int len)
{
	struct relation* rel;
	struct halfrel* self;
	struct halfrel* peer;
/*
	rel = win->orel0;
	if(0 == rel)
	{
		say("@jsonnode_write: %.*s\n", len, buf);
		artery_leafwrite(sc, sf, win, wf, "OK", 2);
		return 0;
	}

	while(1)
	{
		if(0 == rel)break;

		if(_act_ == rel->dsttype){
			self = (void*)&rel->dstchip;
			peer = (void*)&rel->srcchip;
			actorwrite(self, peer, buf, len);
		}

		rel = samesrcnextdst(rel);
	}

	//actorread_all(win);
	artery_leafwrite(sc, sf, win, wf, win->buf, win->len);*/
	return 0;
}
int jsonnode_rootread(struct actor* win, void* wf, void* sc, void* sf, void* buf, int len)
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
