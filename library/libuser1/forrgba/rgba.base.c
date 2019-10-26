#include "libuser.h"
void* allocarena();
//
int preprocess(void*);
int postprocess(void*);
int actorinput_touch(struct arena* win, struct event* ev);




int rgbanode_read(struct arena* win, struct style* stack)
{
	struct style* sty;
	struct relation* rel;
	struct halfrel* self;
	struct halfrel* peer;

	win->vfmt = _3d_;
	preprocess(win);

	rel = win->orel0;
	while(1)
	{
		if(0 == rel)break;

		if(_act_ == rel->dsttype)
		{
			sty = (void*)(rel->srcfoot);
			if(sty){if('#' == sty->i.uc[3])goto next;}

			self = (void*)&rel->dstchip;
			peer = (void*)&rel->srcchip;
			actorread(self, peer, 0, 0, 0, 0);
		}
next:
		rel = samesrcnextdst(rel);
	}

	postprocess(win);
	return 0;
}
int rgbanode_write(struct arena* win, struct style* stack, struct event* ev)
{
	int ret;
	struct style* sty;
	struct halfrel* self;
	struct halfrel* peer;
	struct relation* rel;

	ret = 0;
	rel = win->oreln;
	while(1)
	{
		if(0 == rel)break;

		if(_act_ == rel->dsttype)
		{
			sty = (void*)(rel->srcfoot);
			if(sty){if('#' == sty->i.uc[3])goto next;}

			self = (void*)&rel->dstchip;
			peer = (void*)&rel->srcchip;
			ret = actorwrite(self, peer, 0, 0, ev, 0);
			if(ret)break;
		}
next:
		rel = samesrcprevdst(rel);
	}

	//if('p' == (ev->what&0xff))actorinput_touch(win, ev);
	return ret;
}
int rgbanode_stop(struct arena* win, struct style* sty)
{
	return 0;
}
int rgbanode_start(struct arena* twig, void* tf, struct arena* root, void* rf)
{
	struct halfrel* self;
	struct halfrel* peer;
	struct relation* rel;

	rel = twig->orel0;
	while(1)
	{
		if(0 == rel)break;

		if(_act_ == rel->dsttype){
			self = (void*)&rel->dstchip;
			peer = (void*)&rel->srcchip;
			actorstart(self, peer);
		}

		rel = samesrcnextdst(rel);
	}
	return 0;
}





int rgbanode_search(struct arena* win)
{
	return 0;
}
int rgbanode_modify(struct arena* win)
{
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
