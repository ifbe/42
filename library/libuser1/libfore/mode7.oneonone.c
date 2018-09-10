#include "libuser.h"




int actoroutput_oneonone(struct arena* win, struct style* st)
{
	struct relation* orel;
	struct actor* act;
	struct style* sty;
	struct pinid* pin;

	orel = win->oreln;
	while(1)
	{
		if(0 == orel)break;

		if(_act_ == orel->dsttype)
		{
			act = (void*)(orel->dstchip);
			pin = (void*)(orel->dstfoot);
			//say("%x,%x,%x,%x\n", win, act, sty, pin);
			//say("%x\n", rel);

			sty = (void*)(orel->srcfoot);
			act->onread(win, 0, act, pin);
			break;
		}

		orel = samesrcprevdst(orel);
	}
	return 0;
}
int actorinput_oneonone(struct arena* win, struct event* ev)
{
	struct actor* act;
	struct pinid* pin;
	struct relation* orel;

	orel = win->orel0;
	if(0 == orel)return 0;

	act = (void*)(orel->dstchip);
	pin = (void*)(orel->dstfoot);
	return act->onwrite(act, pin, 0, 0, ev, 0);
}