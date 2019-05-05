#include "libuser.h"
void* allocstyle();
void* allocpinid();
int actorstart(void*, void*, void*, void*, void*, void*);




#define COUNT 3
static u64 want[COUNT] = {
	hex64('1','r','d','c','a','m', 0 , 0 ),
	hex64('3','r','d','c','a','m', 0 , 0 ),
	hex64('f','r','e','e','c','a','m', 0 )
};
static char* args[COUNT] = {
	0,
	0,
	0
};




int cam3d_sread(struct arena* cc, void* cf, struct arena* win, struct style* st)
{
	struct relation* orel;
	struct actor* act;
	struct style* sty;
	struct pinid* pin;
//say("cam3d_sread\n");

	sty = 0;
	orel = cc->orel0;
	while(1)
	{
		if(orel == 0)break;

		if(_act_ == orel->dsttype)
		{
			sty = (void*)(orel->srcfoot);
			if(sty){if('#' == sty->uc[3])goto next;}

			act = (void*)(orel->dstchip);
			pin = (void*)(orel->dstfoot);
			actor_rootread(act, pin, win, sty, 0, 0);
		}
next:
		orel = samesrcnextdst(orel);
	}

	return 1;
}
int cam3d_swrite(struct arena* cc, void* cf, struct arena* win, struct style* stack, struct event* ev)
{
	struct relation* rel;
	struct actor* act;
	struct style* sty;
	struct pinid* pin;
	//say("@cam3d_write\n");

	rel = cc->oreln;
	while(1)
	{
		if(0 == rel)break;

		if(_act_ == rel->dsttype)
		{
			sty = (void*)(rel->srcfoot);
			if(sty){if('#' == sty->uc[3])goto next;}

			act = (void*)(rel->dstchip);
			pin = (void*)(rel->dstfoot);
			actor_rootwrite(act, pin, win, sty, ev, 0);
		}
next:
		rel = samesrcprevdst(rel);
	}
	return 0;
}




int cam3d_stop(struct arena* twig, void* tf, struct arena* root, void* rf)
{
	return 0;
}
int cam3d_start(struct arena* twig, void* tf, struct arena* root, void* rf)
{
	struct relation* rel;
	struct style* sty;
	struct actor* act;
	struct pinid* pin;
say("cam3d_start\n");

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
int cam3d_delete(struct arena* win)
{
	return 0;
}
int cam3d_create(struct arena* win, u8* str)
{
	int j,k;
	struct style* sty;
	struct actor* act;
	struct pinid* pin;
	struct relation* rel;

	for(j=0;j<COUNT;j++)
	{
		act = actorcreate(want[j], args[j]);
		if(0 == act)continue;

		relationcreate(act, 0, _act_, 0, win, 0, _win_, 0);

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

		sty->vr[0] = 100;
		sty->vr[1] = 0;
		sty->vr[2] = 0;

		sty->vf[0] = 0;
		sty->vf[1] = 100;
		sty->vf[2] = 0;

		sty->vu[0] = 0;
		sty->vu[1] = 0;
		sty->vu[2] = 100;
	}
	return 0;
}
