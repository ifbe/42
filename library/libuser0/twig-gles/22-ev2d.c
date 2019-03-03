#include "libuser.h"
int actorstart(void*, void*, void*, void*, void*, void*);




int ev2d_sread(struct arena* cc, void* cf, struct arena* win, struct style* st)
{
	struct relation* orel;
	struct actor* act;
	struct style* sty;
	struct pinid* pin;

	sty = 0;
	orel = cc->orel0;
	while(1)
	{
		if(orel == 0)break;

		if(_act_ == orel->dsttype)
		{
			act = (void*)(orel->dstchip);
			pin = (void*)(orel->dstfoot);
			sty = (void*)(orel->srcfoot);
			act->onread(win, sty, act, pin);
		}

		orel = samesrcnextdst(orel);
	}
	return 1;
}
int ev2d_swrite(struct arena* cc, void* cf, struct arena* win, struct style* sty, struct event* ev)
{
	//say("@ev2d_write\n");
	return 0;
}
int ev2d_stop(struct arena* twig, void* tf, struct arena* root, void* rf)
{
	return 0;
}
int ev2d_start(struct arena* twig, void* tf, struct arena* root, void* rf)
{
	struct relation* rel;
	struct style* sty;
	struct actor* act;
	struct pinid* pin;

	rel = twig->orel0;
	while(1)
	{
		if(0 == rel)break;

		sty = (void*)(rel->srcfoot);
		act = (void*)(rel->dstchip);
		pin = (void*)(rel->dstfoot);
		actorstart(act, pin, twig, sty, root, rf);

		rel = samesrcnextdst(rel);
	}
	return 0;
}
int ev2d_delete(struct arena* win)
{
	return 0;
}
int ev2d_create(struct arena* win, u8* str)
{
	return 0;
}
