#include "libuser.h"




int ev2d_read(struct arena* cc, void* cf, struct arena* win, struct style* st)
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
int ev2d_write(struct arena* cc, void* cf, struct arena* win, struct style* sty, struct event* ev)
{
	//say("@ev2d_write\n");
	return 0;
}




int ev2d_start(struct arena* c, void* cf, struct arena* r, void* rf)
{
	return 0;
}
int ev2d_create(struct arena* win, u8* str)
{
	return 0;
}
