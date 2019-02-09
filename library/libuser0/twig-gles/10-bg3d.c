#include "libuser.h"
#define _bg_ hex32('b','g',0,0)
void* allocstyle();
void* allocpinid();
int actorstart(void*, void*, void*, void*);




u64 want[2] = {
	hex64('s','k','y','s','p','h','.','.'),
	hex64('t','e','r','r','i','a','n', 0)
};




int bg3d_read(struct arena* cc, void* cf, struct arena* win, struct style* stack)
{
	int j;
	struct relation* rel;
	struct actor* act;
	struct style* sty;
	struct pinid* pin;
	win->vfmt = _3d_;

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
int bg3d_write(struct arena* win, struct style* stack, struct event* ev)
{
	return 0;
}




int bg3d_start(struct arena* c, void* cf, struct arena* r, void* rf)
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
int bg3d_create(struct arena* win, u8* str)
{
	int j,k;
	struct style* sty;
	struct actor* act;
	struct pinid* pin;
	struct relation* rel;

	for(j=0;j<2;j++)
	{
		act = actorcreate(want[j], 0);
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

		sty->vc[0] = 0;
		sty->vc[1] = 0;
		sty->vc[2] = 0;

		sty->vr[0] = 1000*1000;
		sty->vr[1] = 0;
		sty->vr[2] = 0;

		sty->vf[0] = 0;
		sty->vf[1] = 1000*1000;
		sty->vf[2] = 0;

		sty->vu[0] = 0;
		sty->vu[1] = 0;
		sty->vu[2] = 1000*1000;
	}
	return 0;
}
