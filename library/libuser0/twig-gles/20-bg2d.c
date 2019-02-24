#include "libuser.h"
void* allocstyle();
void* allocpinid();
void defaultstyle_2in3(struct style*);
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
	int j;
	struct style* sty;
	struct actor* act;
	struct pinid* pin;
	struct relation* rel;

	for(j=0;j<1;j++)
	{
		act = actorcreate(hex64('s','p','h','e','r','e', 0, 0), 0);
		if(0 == act)continue;

		relationcreate(act, 0, _act_, win, 0, _win_);

		rel = act->irel0;
		if(0 == rel)continue;

		sty = allocstyle();
		if(0 == sty)continue;

		pin = allocpinid();
		if(0 == pin)continue;

		rel->srcfoot = (u64)sty;
		rel->dstfoot = (u64)pin;

		defaultstyle_2in3(sty);
	}
	return 0;
}
