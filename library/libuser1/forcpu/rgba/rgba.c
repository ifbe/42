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
	int s = win->fbwidth>>2;
	int len = s*h;
	u32* buf = (u32*)(win->rgbabuf);

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




int rgbanode_read(_sup* wnd,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	background_pixel(wnd);

	struct relation* rel = wnd->orel0;
	while(1)
	{
		if(0 == rel)break;

		if(_ent_ == rel->dsttype)
		{
			struct style* sty = (void*)(rel->srcfoot);
			if(sty){if('#' == sty->i.uc[3])goto next;}

			stack[sp+0].pchip = rel->psrcchip;
			stack[sp+0].pfoot = rel->psrcfoot;
			stack[sp+0].flag = rel->srcflag;
			stack[sp+1].pchip = rel->pdstchip;
			stack[sp+1].pfoot = rel->pdstfoot;
			stack[sp+1].flag = rel->dstflag;
			entityread(stack[sp+1].pchip, stack[sp+1].flag, stack,sp+2, arg,key, buf,len);
		}
next:
		rel = samesrcnextdst(rel);
	}

	foreground_pixel(wnd);
	return 0;
}
int rgbanode_write(_sup* wnd,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	//struct event* ev = buf;
	//say("@rgbanode_write:%.8s,%llx\n", &ev->what, ev->why);

	int ret = 0;
	struct relation* rel = wnd->oreln;
	while(1)
	{
		if(0 == rel)break;

		if(_ent_ == rel->dsttype)
		{
			struct style* sty = (void*)(rel->srcfoot);
			if(sty){if('#' == sty->i.uc[3])goto next;}

			stack[sp+0].pchip = rel->psrcchip;
			stack[sp+0].pfoot = rel->psrcfoot;
			stack[sp+0].flag = rel->srcflag;
			stack[sp+1].pchip = rel->pdstchip;
			stack[sp+1].pfoot = rel->pdstfoot;
			stack[sp+1].flag = rel->dstflag;
			ret = entitywrite(stack[sp+1].pchip, stack[sp+1].flag, stack,sp+2, arg,key, buf,len);
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
