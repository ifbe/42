#include "libuser.h"




int actoroutput_deliver(struct arena* win, struct style* st)
{
	struct relation* rel;
	struct actor* act;
	struct style* sty;
	struct pinid* pin;

	rel = win->irel0;
	while(1)
	{
		if(rel == 0)break;

		if(rel->srctype == _act_)
		{
			act = (void*)(rel->srcchip);
			sty = (void*)(rel->dstfoot);
			pin = (void*)(rel->srcfoot);
			//say("%x,%x,%x,%x\n", win, act, sty, pin);
			//say("%x\n", rel);

			act->onread(win, sty, act, pin);
		}

		rel = samedstnextsrc(rel);
	}
	return 0;
}
int actorinput_deliver(struct arena* win, struct event* ev)
{
	struct actor* act;
	struct pinid* pin;
	struct relation* rel;
	struct relation* tmp;

	rel = win->irel0;
	if(0 == rel)return 0;

	while(1)
	{
		tmp = samedstnextsrc(rel);
		if(tmp == 0)break;

		rel = tmp;
	}

	act = (void*)(rel->srcchip);
	pin = (void*)(rel->srcfoot);
	return act->onwrite(act, pin, 0, 0, ev, 0);
}