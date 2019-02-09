#include "libuser.h"
void* allocstyle();
void* allocpinid();
int actorstart(void*, void*, void*, void*);




int bg2d_read(struct arena* cc, void* cf, struct arena* win, struct style* stack)
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
int bg2d_write(struct arena* win, struct style* stack, struct event* ev)
{
	return 0;
}




int bg2d_start(struct arena* c, void* cf, struct arena* r, void* rf)
{
	struct relation* rel;
	struct style* sty;
	struct actor* act;
	struct pinid* pin;

	rel = c->orel0;
	while(1)
	{
		if(0 == rel)break;

		sty = (void*)(rel->srcfoot);
		act = (void*)(rel->dstchip);
		pin = (void*)(rel->dstfoot);
		actorstart(r, sty, act, pin);

		rel = samesrcnextdst(rel);
	}
	return 0;
}
int bg2d_create(struct arena* win, u8* str)
{
	//struct actor* act = actorcreate(hex32('2','0','4','8'), 0);
	//if(0 == act)return 0;

	//relationcreate(act, 0, _act_, win, 0, _win_);
	return 0;
}
