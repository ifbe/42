#include "actor.h"
#define _WS_ hex32('W','S',0,0)
#define _ws_ hex32('w','s',0,0)
void lib1d_create(void*, void*);
void lib1d_delete();
void lib2d_create(void*, void*);
void lib2d_delete();
void lib3d_create(void*, void*);
void lib3d_delete();
void lib4d_create(void*, void*);
void lib4d_delete();
//vkbd
int background(struct arena* win);
int foreground(struct arena* win);
int vkbd_read(      struct arena* win, struct style* sty);
int actorinput_vkbd(struct arena* win, struct event* ev);
//
int actoroutput_void(struct arena* win, struct style* sty);
int actorinput_void( struct arena* win, struct event* ev);
//menu
int actoroutput_menu(struct arena* win, struct style* sty);
int actorinput_menu( struct arena* win, struct event* ev);
//mode0
int actoroutput_overview(struct arena* win, struct style* sty);
int actorinput_overview( struct arena* win, struct event* ev);
//mode1
int actoroutput_detail(struct arena* win, struct style* sty);
int actorinput_detail( struct arena* win, struct event* ev);
//mode2
int actoroutput_edit(struct arena* win, struct style* sty);
int playwith2d(struct arena* win, struct event* ev);
int playwith3d(struct arena* win, struct event* ev);
//mode3
int actoroutput_posture(struct arena* win, struct style* sty);
int actorinput_camera( struct arena* win, struct event* ev);
//mode4
int actoroutput_deliver(struct arena* win, struct style* sty);
int actorinput_deliver( struct arena* win, struct event* ev);
//mode5
int actoroutput_oneonone(struct arena* win, struct style* sty);
int actorinput_oneonone( struct arena* win, struct event* ev);
//
int parsexml_detail(void*, int, void*, void*, void*, void*);
int touch_explain(struct arena* win, struct event* ev);



static struct arena* arena = 0;
static struct actor* actor = 0;
static struct style* style = 0;
static struct pinid* pinid = 0;
static int actlen = 0;
static int pinlen = 0;
void* allocactor()
{
	int j,max;
	struct actor* act;

	max = 0x100000 / sizeof(struct actor);
	for(j=0;j<max;j++)
	{
		act = &actor[j];
		if(0 == act->type)return act;
	}
	return 0;
}
void* allocactor_reverse()
{
	int j,max;
	struct actor* act;

	max = 0x100000 / sizeof(struct actor);
	for(j=max-1;j>=0;j--)
	{
		act = &actor[j];
		if(0 == act->type)return act;
	}
	return 0;
}
void* allocpinid()
{
	struct pinid* pin = (void*)pinid + pinlen;
	pinlen += sizeof(struct pinid);

	pin->flag00 = 0;
	pin->flag01 = 1;
	pin->flag02 = 2;
	pin->flag03 = 3;

	return pin;
}




int actorinput_special(struct arena* win, struct event* ev)
{
	int ret, val;

	val = 0;
	if(_kbd_ == ev->what)
	{
		if(ev->why == 0xfb)val = 'l';
		else if(ev->why == 0xfc)val = 'r';
	}
	else if(_joy_ == ev->what)
	{
		ret = ((ev->why)>>32)&0xffff;
		if(_ll_ == ret)val = 'l';
		else if(_rr_ == ret)val = 'r';
	}

	if(('l' == val)|('r' == val))
	{
		win->voidtype = 1;
		return 1;
	}
	return 0;
}
void actorinput_touch(struct arena* win, struct event* ev)
{
	int x,y,z,btn;
	if('p' != (ev->what & 0xff))return;

	x = (ev->why)&0xffff;
	y = ((ev->why)>>16)&0xffff;
	z = ((ev->why)>>32)&0xffff;
	btn = ((ev->why)>>48)&0xffff;

	if('l' == btn)btn = 10;
	else if('r' == btn)btn = 11;
	else if(10 < btn)return;

	if(hex32('p','@',0,0) == ev->what)
	{
		win->touchmove[btn].x = x;
		win->touchmove[btn].y = y;
	}
	else if(hex32('p','+',0,0) == ev->what)
	{
		win->touchdown[btn].x = win->touchmove[btn].x = x;
		win->touchdown[btn].y = win->touchmove[btn].y = y;
		win->touchdown[btn].z = 1;
	}
	else if(hex32('p','-',0,0) == ev->what)
	{
		win->touchdown[btn].z = 0;
	}
}
int actorwrite_ev(struct event* ev)
{
	int ttt,ret;
	struct arena* win = (void*)(ev->where);
	if(0 == win)win = &arena[0];

	if(_drag_ == ev->what)
	{
		say("%s\n", (void*)(ev->why));
		return 0;
	}

	//check vkbd
	ret = actorinput_vkbd(win, ev);
	if(0 != ret)goto theend;

	//check menu
	ret = actorinput_special(win, ev);
	if(0 != ret)goto theend;

	//
	ttt = win->voidtype;
	ret = win->menutype;
	if(0 == ttt)actorinput_void(win, ev);
	else if(1 == ttt)actorinput_menu(win, ev);
	else if(0 == ret)actorinput_overview(win, ev);
	else if(1 == ret)actorinput_detail(win, ev);
	else if(2 == ret)
	{
		if(_vbo_ == win->fmt)playwith3d(win, ev);
		else playwith2d(win, ev);
	}
	else if(3 == ret)actorinput_camera(win, ev);
	else if(4 == ret)actorinput_deliver(win, ev);
	else if(5 == ret)actorinput_oneonone(win, ev);

theend:
	if('p' == (ev->what&0xff))actorinput_touch(win, ev);
	win->enq += 1;
	return 0;
}
int actorread_all(struct arena* win)
{
/*
	//cli silent
	if(win->fmt == _cli_)
	{
		if(win->edit)return 0;
	}
*/
	int ttt, ret;

	//bg
	background(win);

	//context
	ttt = win->voidtype;
	ret = win->menutype;
	if(0 == ttt)actoroutput_void(win, 0);
	else if(1 == ttt)actoroutput_menu(win, 0);
	else if(0 == ret)actoroutput_overview(win, 0);
	else if(1 == ret)actoroutput_detail(win, 0);
	else if(2 == ret)actoroutput_edit(win, 0);
	else if(3 == ret)actoroutput_posture(win, 0);
	else if(4 == ret)actoroutput_deliver(win, 0);
	else if(5 == ret)actoroutput_oneonone(win, 0);

	//vkbd
	vkbd_read(win, 0);

	//fg
	foreground(win);
	return 0;
}




int actorwrite(void* dc,void* df,void* sc,void* sf,void* buf,int len)
{
	struct relation* rel;
	struct actor* act;
	struct arena* win;
	if(0 == dc)return actorwrite_ev(buf);

	act = dc;
	act->onwrite(act, df, sc, sf, buf, len);

	rel = act->orel;
	while(1)
	{
		if(0 == rel)break;
		if(_win_ == rel->dsttype)
		{
			win = (void*)(rel->dstchip);
			win->enq += 1;
		}
		rel = samesrcnextdst(rel);
	}
	return 0;
}
int actorread(void* dc,void* df,void* sc,void* sf,void* buf,int len)
{
	actorread_all(dc);
	return 0;
}
int actorstop(struct actor* act, struct pinid* pin)
{
	if(0 == act)return 0;
	act->onstop(act, pin);
	return 0;
}
int actorstart(struct actor* act, struct pinid* pin)
{
	if(0 == act)return 0;
	act->onstart(act, pin);
	return 0;
}
int actordelete(struct actor* act, u8* buf)
{
	if(0 == act)return 0;
	if(_orig_ == act->type)return 0;
	if(_copy_ == act->type)return 0;
	act->ondelete(act, buf);

	if(_ORIG_ == act->type)act->type = _orig_;
	else if(_COPY_ == act->type)act->type = _copy_;
	return 0;
}
int actorcreate(struct actor* act, u8* buf)
{
	if(0 == act)return 0;
	if(_ORIG_ == act->type)return 0;
	if(_COPY_ == act->type)return 0;

	act->oncreate(act, buf);
	if(_orig_ == act->type)act->type = _ORIG_;
	else if(_copy_ == act->type)act->type = _COPY_;
	return 0;
}
void* actorchoose(u8* buf, int len)
{
	//say("%.*s\n", len, buf);
	int j;
	struct actor* act = 0;
	u64 name = 0;
	int id = 0;
	u8* data = 0;
	int dl = 0;

	parsexml_detail(buf, len, &name, &id, &data, &dl);
	//say("%.*s\n", len, buf);
	//say("%llx, %x\n", name, id);
	//say("%.*s\n", dl, data);

	act = 0;
	for(j=0;j<0x100;j++)
	{
		if(0 == actor[j].name)break;
		if(0 > id)break;

		if(name == actor[j].name)
		{
			if(0 == id)
			{
				act = &actor[j];
				actorcreate(act, data);
				break;
			}
		}
	}

	return 0;
}
void* actorlist(u8* buf, int len)
{
	int j,k;
	u8* p;
	if(0 == buf)
	{
		for(j=0;j<0x100;j++)
		{
			if(0 == actor[j].name)break;
			say("[%03x]: %.4s,%.8s\n", j, &actor[j].type, &actor[j].name);
		}
		if(0 == j)say("empty actor\n");
	}
	else
	{
		for(j=0;j<0x100;j++)
		{
			if(0 == actor[j].name)break;
			p = (void*)(&actor[j].name);

			for(k=0;k<8;k++)
			{
				if((0 == p[k])|(0x20 >= buf[k]))return &actor[j];
				if(buf[k] != p[k])break;
			}
		}
	}
	return 0;
}




void freeactor()
{
	//say("[c,f):deleteing actor\n");

	lib4d_delete();
	lib3d_delete();
	lib2d_delete();
	lib1d_delete();

	pinid = 0;
	style = 0;
	actor = 0;
	arena = 0;
}
void initactor(u8* addr)
{
	int j;
	arena = (void*)(addr+0x000000);
	actor = (void*)(addr+0x100000);
	style = (void*)(addr+0x200000);
	pinid = (void*)(addr+0x300000);

#define max (0x100000/sizeof(struct actor))
	for(j=0;j<max;j++)actor[j].tier = _act_;

	lib1d_create(addr, 0);
	lib2d_create(addr, 0);
	lib3d_create(addr, 0);
	lib4d_create(addr, 0);

	//say("[c,f):createed actor\n");
}
