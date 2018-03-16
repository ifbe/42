#include "actor.h"
int term_write(void*);
int login_write(void*, void*);
int relation_swap(void*, void*);
int relation_destory(void*);
void* samepinprevchip(void*);
void* samepinnextchip(void*);
void* samechipprevpin(void*);
void* samechipnextpin(void*);
void* relation_read(u64);
void touch_explain(struct arena* win, struct event* ev);




//
static u64 pointenter[10];
static u64 pointleave[10];
static int pointcount=0;
static int pointmax=0;
//
static struct arena* arena = 0;
static struct actor* actor = 0;
static struct style* style = 0;
static struct compo* compo = 0;
void helpin_create(void* addr)
{
	arena = addr + 0x000000;
	actor = addr + 0x100000;
	style = addr + 0x000000;
	compo = addr + 0x100000;
}




int point_explain(struct arena* win, struct event* ev)
{
	struct relation* reltop;
	struct relation* relwow;
	struct style* stytop;
	struct style* stywow;
	int absx, absy;
	int x = (ev->why)&0xffff;
	int y = ((ev->why)>>16)&0xffff;
	int z = ((ev->why)>>32)&0xffff;
	int btn = ((ev->why)>>48)&0xffff;

	reltop = win->irel;
	if(reltop == 0)return 1;
	while(1)
	{
		relwow = samepinnextchip(reltop);
		if(relwow == 0)break;

		reltop = relwow;
	}
	stytop = (void*)(reltop->destfoot);

	stywow = 0;
	relwow = reltop;
	while(1)
	{
		if(relwow == 0)break;
		stywow = (void*)(relwow->destfoot);

		if(x > stywow->cx)absx = x - (stywow->cx);
		else absx = (stywow->cx) - x;
		if(y > stywow->cy)absy = y - (stywow->cy);
		else absy = (stywow->cy) - y;

		if((absx <= stywow->rx)&&(absy <= stywow->fy))break;

		relwow = samepinprevchip(relwow);
		stywow = 0;
	}
	//say("%x,%x,%x,%x\n",reltop,stytop,stytop,stywow);

	if('f' == btn)
	{
		stytop->rx = (stytop->rx)*17/16;
		stytop->fy = (stytop->fy)*17/16;
		stytop->uz = (stytop->uz)*17/16;
		return 0;
	}
	else if('b' == btn)
	{
		stytop->rx = (stytop->rx)*15/16;
		stytop->fy = (stytop->fy)*15/16;
		stytop->uz = (stytop->uz)*15/16;
		return 0;
	}

	if(hex32('p','@',0,0) == ev->what)
	{
		if(btn > 10)btn = 10;
		if(0 != win->touchdown[10].z)
		{
			stytop->cx += x - (win->touchmove[btn].x);
			stytop->cy += y - (win->touchmove[btn].y);
			//say("%x,%x\n", stytop->cx, stytop->cy);
		}
	}
	else if(hex32('p','+',0,0) == ev->what)
	{
		if(stywow != 0)relation_swap(reltop, relwow);
	}
	return 1;
}
int delete_topone(struct arena* win)
{
	struct relation* rel;
	struct relation* tmp;
	if(win == 0)return 0;
	if(win->irel == 0)return 0;

	rel = win->irel;
	while(1)
	{
		tmp = samepinnextchip(rel);
		if(tmp == 0)break;

		rel = tmp;
	}
	relation_destory(rel);
	return 1;
}
int actorinput(struct arena* win, struct event* ev)
{
	u64 why,what;
	int x,y,ret;
	struct actor* act;
	struct compo* com;
	struct relation* rel;
	struct relation* tmp;
	why = ev->why;
	what = ev->what;

	if(0 == win->irel)
	{
		if(_cli_ == win->fmt)term_write(ev);
		else login_write(win, ev);
		goto lastword;
	}

	if(_kbd_ == what)
	{
		ret=0;
		if(why == 0xfb)ret = 11;
		else if(why == 0xfc)ret = 12;

		if(ret != 0)
		{
			if(ret == win->flag0)win->flag0 = 0;
			else win->flag0 = ret;
			goto lastword;
		}
	}

	if(hex32('p','-',0,0) == ev->what)
	{
		x = (ev->why)&0xffff;
		y = ((ev->why)>>16)&0xffff;
		if(win->w < win->h)ret = win->h;
		else ret = win->w;

		ret >>= 4;
		if((x+ret > win->w) && (y+ret > win->h))
		{
			if(0 == win->flag0)win->flag0 = 12;
			else if(12 == win->flag0)win->flag0 = 11;
			else win->flag0 = 0;
			goto lastword;
		}
	}

	if(11 == win->flag0)
	{
		login_write(win, ev);
		goto lastword;
	}

	if(12 == win->flag0)
	{
		if(what == _char_)delete_topone(win);
		else point_explain(win, ev);
		goto lastword;
	}

	rel = win->irel;
	while(1)
	{
		tmp = samepinnextchip(rel);
		if(tmp == 0)break;

		rel = tmp;
	}

	act = (void*)(rel->selfchip);
	com = (void*)(rel->selffoot);
	act->onwrite(act, com, ev);

lastword:
	if('p' == (ev->what & 0xff))touch_explain(win, ev);
	return 1;
}