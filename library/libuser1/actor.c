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
//
int parsexml_detail(void*, int, void*, void*, void*, void*);
int actorinput(void*, void*);
int actoroutput(void*);
int touch_explain(struct arena* win, struct event* ev);



static struct arena* arena = 0;
static struct actor* actor = 0;
static struct style* style = 0;
static struct pinid* pinid = 0;
static int actlen = 0;
static int pinlen = 0;
void* allocactor()
{
	int j;
	struct actor* act;
	while(1)
	{
		act = &actor[j];
		if(0 == act->type)break;

		j++;
		if(j >= 0x100)return 0;
	}
	return act;
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
int actorread(void* dc,void* df,void* sc,void* sf)
{
	actoroutput(dc);
	return 0;
}
int actorwrite(void* dc,void* df,void* sc,void* sf,void* buf,int len)
{
	struct relation* rel;
	struct event* ev;
	struct actor* act;
	struct arena* win;
	if(dc != 0)
	{
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
	}
	else
	{
		win = sc;
		ev = buf;
		actorinput(win, ev);

		if('p' == (ev->what&0xff))touch_explain(win, ev);
		win->enq += 1;
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
			say("[%03x]: %.8s,%.8s\n", j, &actor[j].type, &actor[j].name);
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




int actorevent(struct event* ev)
{
	int j;
	struct actor* act;
	struct arena* win = (void*)(ev->where);
	if(0 == win)return 0;

	actorinput(win, ev);
	if('p' == (ev->what&0xff))
	{
		touch_explain(win, ev);
		win->dirty = 1;
		if('@' == ((ev->what>>8)&0xff))return 0;
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
