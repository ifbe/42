#include "libuser.h"
void* allocstyle();
void* allocpinid();
int actorstart(void*, void*, void*, void*, void*, void*);




#define COUNT 1
static u64 want[COUNT] = {
	hex64('o','v','e','r','v','i','e','w')
};
static u8 flag[COUNT] = {
	0,
};




int ui2d_sread(struct arena* cc, void* cf, struct arena* win, struct style* stack)
{
	struct relation* rel;
	struct actor* act;
	struct style* sty;
	struct pinid* pin;
	win->vfmt = _ui2d_;

	rel = cc->orel0;
	while(1)
	{
		if(0 == rel)break;

		if(_act_ == rel->dsttype)
		{
			sty = (void*)(rel->srcfoot);
			if(sty){if('#' == sty->uc[3])goto next;}

			act = (void*)(rel->dstchip);
			pin = (void*)(rel->dstfoot);
			actor_rootread(act, pin, win, sty, 0, 0);
		}
next:
		rel = samesrcnextdst(rel);
	}
	return 0;
}
int ui2d_swrite(struct arena* cc, void* cf, struct arena* win, struct style* stack, struct event* ev)
{
	int ret;
	struct relation* rel;
	struct actor* act;
	struct style* sty;
	struct pinid* pin;

	ret = 0;
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
			ret = actor_rootwrite(act, pin, win, 0, ev, 0);
			if(ret)break;
		}
next:
		rel = samesrcprevdst(rel);
	}

	return ret;
}
int ui2d_cread(struct arena* win, struct style* stack, struct actor* sc, struct pinid* sf)
{
	return 0;
}
int ui2d_cwrite(struct arena* win, struct style* stack, struct actor* sc, struct pinid* sf, int flag)
{
	struct relation* rel;
	struct style* sty;
	struct actor* act;

	rel = win->orel0;
	while(1)
	{
		if(0 == rel)break;
		if(_act_ == rel->dsttype)
		{
			sty = (void*)(rel->srcfoot);
			if(sty){sty->uc[3] = 0;}
		}

		rel = samesrcnextdst(rel);
	}
	return 0;
}
int ui2d_stop(struct arena* twig, void* tf, struct arena* root, void* rf)
{
	return 0;
}
int ui2d_start(struct arena* twig, void* tf, struct arena* root, void* rf)
{
	struct relation* rel;
	struct style* sty;
	struct actor* act;
	struct pinid* pin;

	rel = twig->orel0;
	while(1)
	{
		if(0 == rel)break;

		if(_act_ == rel->dsttype)
		{
			sty = (void*)(rel->srcfoot);

			act = (void*)(rel->dstchip);
			pin = (void*)(rel->dstfoot);
			actorstart(act, pin, twig, sty, root, rf);
		}

		rel = samesrcnextdst(rel);
	}
	return 0;
}
int ui2d_delete(struct arena* win)
{
	return 0;
}
int ui2d_create(struct arena* win, void* str)
{
	int j,k;
	struct style* sty;
	struct actor* act;
	struct pinid* pin;
	struct relation* rel;

	for(j=0;j<COUNT;j++)
	{
		act = actorcreate(want[j], 0);
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
		sty->uc[3] = flag[j];

		sty->vr[0] = 1.0;
		sty->vr[1] = 0;
		sty->vr[2] = 0;

		sty->vf[0] = 0;
		sty->vf[1] = 1.0;
		sty->vf[2] = 0;

		sty->vu[0] = 0;
		sty->vu[1] = 0;
		sty->vu[2] = 1.0;
	}
	return 0;
}
