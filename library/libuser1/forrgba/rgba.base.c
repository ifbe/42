#include "libuser.h"
void* allocsupply();
//
int preprocess(void*);
int postprocess(void*);
int entityinput_touch(struct supply* win, struct event* ev);




void background_pixel(struct supply* win)
{
	int x;
	int w = win->width;
	int h = win->height;
	int s = win->stride;
	int len = s*h;
	u32* buf = (u32*)(win->rawbuf);

	for(x=0;x<len;x++)buf[x] = 0xff000000;
}
void foreground_pixel(struct supply* win)
{
/*	if((win->theone) | (win->edit) | (0 == win->irel))
	{
		drawline(win, 0xffffff, 0, h/2, w, h/2);
		drawline(win, 0xffffff, w/2, 0, w/2, h);
	}*/
}




int rgbanode_read(struct supply* win, struct event* ev)
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

		if(_ent_ == rel->dsttype)
		{
			sty = (void*)(rel->srcfoot);
			if(sty){if('#' == sty->i.uc[3])goto next;}

			self = (void*)&rel->dstchip;
			peer = (void*)&rel->srcchip;
			entityread(self, peer, 0, 0, 0, 0);
		}
next:
		rel = samesrcnextdst(rel);
	}

	foreground_pixel(win);
	return 0;
}
int rgbanode_write(struct supply* win, struct event* ev)
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

		if(_ent_ == rel->dsttype)
		{
			sty = (void*)(rel->srcfoot);
			if(sty){if('#' == sty->i.uc[3])goto next;}

			self = (void*)&rel->dstchip;
			peer = (void*)&rel->srcchip;
			ret = entitywrite(self, peer, 0, 0, ev, 0);
			if(ret)break;
		}
next:
		rel = samesrcprevdst(rel);
	}

	//if('p' == (ev->what&0xff))entityinput_touch(win, ev);
	return ret;
}
int rgbanode_discon(struct supply* win, struct style* sty)
{
	return 0;
}
int rgbanode_linkup(struct supply* twig, void* tf, struct supply* root, void* rf)
{
	return 0;
}





int rgbanode_search(struct supply* win)
{
	return 0;
}
int rgbanode_modify(struct supply* win)
{
	return 0;
}
int rgbanode_delete(struct supply* win)
{
	return 0;
}
int rgbanode_create(u64 type, void* addr)
{
	return 0;
}
