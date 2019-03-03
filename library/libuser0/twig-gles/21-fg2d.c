#include "libuser.h"
void drawborder2d(struct arena* win, struct style* sty, void* name);
void carveborder2d(struct arena* win, struct style* sty, void* name);




int fg2d_sread(struct arena* cc, void* cf, struct arena* win, struct style* stack)
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

		if(_act_ == rel->dsttype)
		{
			act = (void*)(rel->dstchip);
			sty = (void*)(rel->srcfoot);
			pin = (void*)(rel->dstfoot);
			act->onread(win, sty, act, pin);

			//if(_vbo_ == win->fmt)carveborder2d(win, sty, &act->name);
			//else drawborder2d(win, sty, &act->name);
		}

		rel = samesrcnextdst(rel);
	}
	return 0;
}
int fg2d_swrite(struct arena* cc, void* cf, struct arena* win, struct style* stack, struct event* ev)
{
	struct relation* rel;
	struct actor* act;
	struct style* sty;
	struct pinid* pin;
	//say("@fg2d_write\n");

	rel = cc->oreln;
	while(1)
	{
		if(0 == rel)break;

		if(_act_ == rel->dsttype)
		{
			act = (void*)(rel->dstchip);
			sty = (void*)(rel->srcfoot);
			pin = (void*)(rel->dstfoot);
			act->onwrite(act, pin, win, sty, ev, 0);
		}

		rel = samesrcprevdst(rel);
	}
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
