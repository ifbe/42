#include "libuser.h"
void* allocarena();
//
int preprocess(void*);
int postprocess(void*);
int actorinput_touch(struct arena* win, struct event* ev);




void background_pixel(struct arena* win)
{
	int x;
	int w = win->width;
	int h = win->height;
	int s = win->stride;
	int len = s*h;
	u32* buf = (u32*)(win->buf);

	for(x=0;x<len;x++)buf[x] = 0xff000000;
}
void foreground_pixel(struct arena* win)
{
/*	if((win->theone) | (win->edit) | (0 == win->irel))
	{
		drawline(win, 0xffffff, 0, h/2, w, h/2);
		drawline(win, 0xffffff, w/2, 0, w/2, h);
	}*/
}




int rgbanode_read(struct arena* win, struct event* ev)
{
	struct style* sty;
	struct relation* rel;
	struct halfrel* self;
	struct halfrel* peer;

	background_pixel(win);

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

	foreground_pixel(win);
	return 0;
}
int rgbanode_write(struct arena* win, struct event* ev)
{
	int ret;
	struct style* sty;
	struct halfrel* self;
	struct halfrel* peer;
	struct relation* rel;
	say("@rgbanode_write:%.8s,%llx\n", &ev->what, ev->why);

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
int rgbanode_create(u64 type, void* addr)
{
	return 0;
}
