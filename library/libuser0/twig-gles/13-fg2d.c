#include "libuser.h"




int fg2d_read(struct arena* cc, void* cf, struct arena* win, struct style* stack)
{
	int j;
	struct relation* rel;
	struct actor* act;
	struct style* sty;
	struct pinid* pin;
	win->vfmt = _2d_;

	rel = cc->orel0;
	while(1)
	{
		if(0 == rel)break;

		act = (void*)(rel->dstchip);
		sty = (void*)(rel->srcfoot);
		pin = (void*)(rel->dstfoot);
		act->onread(win, sty, act, pin);

		rel = samesrcnextdst(rel);
	}
	return 0;
}
int fg2d_write(struct arena* win, struct style* stack, struct event* ev)
{
	return 0;
}




int fg2d_start(struct arena* c, void* cf, struct arena* r, void* rf)
{
	return 0;
}
int fg2d_create(struct arena* win, u8* str)
{
	return 0;
}
