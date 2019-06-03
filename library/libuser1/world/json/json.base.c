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
	struct relation* rel;
	struct halfrel* self;
	struct halfrel* peer;

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
			actor_rootwrite(self, peer, buf, len);
		}

		rel = samesrcnextdst(rel);
	}

	//actorread_all(win);
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