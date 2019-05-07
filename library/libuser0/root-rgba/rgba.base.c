#include "libuser.h"
void* allocarena();
int actorstart(void*, void*, void*, void*, void*, void*);
//
int preprocess(void*);
int postprocess(void*);
int actorinput_touch(struct arena* win, struct event* ev);




int rgbanode_sread(struct arena* win, struct style* stack)
{
	struct relation* rel;
	struct actor* act;
	struct style* sty;
	struct pinid* pin;

	win->vfmt = _3d_;
	preprocess(win);

	rel = win->orel0;
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

	postprocess(win);
	return 0;
}
int rgbanode_swrite(struct arena* win, struct style* stack, struct event* ev)
{
	int ret;
	struct relation* rel;
	struct actor* act;
	struct style* sty;
	struct pinid* pin;

	ret = 0;
	rel = win->oreln;
	while(1)
	{
		if(0 == rel)break;

		if(_act_ == rel->dsttype)
		{
			sty = (void*)(rel->srcfoot);
			if(sty){if('#' == sty->uc[3])goto next;}

			act = (void*)(rel->dstchip);
			pin = (void*)(rel->dstfoot);
			ret = actor_rootwrite(act, pin, win, sty, ev, 0);
			if(ret)break;
		}
next:
		rel = samesrcprevdst(rel);
	}

	if('p' == (ev->what&0xff))actorinput_touch(win, ev);
	return ret;
}




int rgbanode_stop(struct arena* win, struct style* sty)
{
	return 0;
}
int rgbanode_start(struct arena* twig, void* tf, struct arena* root, void* rf)
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
			actorstart(act, pin, twig, sty, twig, 0);
		}

		rel = samesrcnextdst(rel);
	}
	return 0;
}




int rgbanode_delete(struct arena* win)
{
	return 0;
}
void* rgbanode_create(u64 type, void* addr)
{
	struct arena* win = allocarena();
	if(0 == win)return 0;

	win->fmt = hex64('r','g','b','a','8','8','8','8');
	win->width = win->fbwidth = 1024;
	win->height = win->fbheight = 768;
	return win;
}
