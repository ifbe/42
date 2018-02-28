#include "actor.h"
void lib1d_create(void*, void*);
void lib1d_delete();
void lib2d_create(void*, void*);
void lib2d_delete();
void lib3d_create(void*, void*);
void lib3d_delete();
void lib4d_create(void*, void*);
void lib4d_delete();
//
void* arenastart(u64, u64);
void arenastop(void*);
int actorinput(void*, void*);
int actoroutput(void*);
//
void* samepinprevchip(void*);
void* samepinnextchip(void*);
void* samechipprevpin(void*);
void* samechipnextpin(void*);
void* relation_read(u64);
void relation_write(
	void* uchip, void* ufoot, u64 utype,
	void* bchip, void* bfoot, u64 btype
);



static struct arena* arena = 0;
static struct actor* actor = 0;
static struct style* style = 0;
static struct compo* compo = 0;
static int winlen = 0;
static int actlen = 0;
static int stylen = 0;
static int comlen = 0;




void act_add()
{
}
void act_del()
{
}
void act_at(struct arena* win, struct actor* act)
{
	int w = win->w;
	int h = win->h;
	int d = (w+h) / 2;
	struct style* sty;
	struct compo* com;

	sty = (void*)style + stylen;
	stylen += sizeof(struct style);
	sty->cx = w /2;
	sty->cy = h /2;
	sty->cz = 0.0;
	sty->rx = w *49/100;
	sty->ry = 0.0;
	sty->rz = 0.0;
	sty->fx = 0.0;
	sty->fy = h *49/100;
	sty->fz = 0.0;
	sty->ux = 0.0;
	sty->uy = 0.0;
	sty->uz = d *49/100;

	com = (void*)compo + comlen;
	comlen += sizeof(struct compo);
	com->flag00 = 0;
	com->flag01 = 1;
	com->flag02 = 2;
	com->flag03 = 3;

	relation_write(
		win, sty, _win_,
		act, com, _act_
	);
}




void win_add(u64 why, u64 where)
{
	void* ret = arenastart(why, where);
	if(ret == 0)
	{
		say("error@w+\n");
		return;
	}
}
void win_del(u64 why, u64 where)
{
	void* ret = (void*)where;
	arenastop(ret);
}
void win_at(u64 why, u64 where)
{
}




int actorread()
{
	int j;
	for(j=0;j<16;j++)
	{
		if(0 == arena[j].type)break;
		actoroutput(&arena[j]);
	}
	return 0;
}
int actorwrite(struct event* ev)
{
	int ret;
	struct arena* win;
	u64 why, what, where, when;

	why = ev->why;
	what = ev->what;
	where = ev->where;
	//say("%x,%x,%x\n", why, what, where);

	//window event
	if('w' == (what&0xff))
	{
		if(hex32('w','+',0,0) == what)win_add(why, where);
		else if(hex32('w','-',0,0) == what)win_del(why, where);
		else if(hex32('w','@',0,0) == what)win_at(why, where);
		return 0;
	}

	//no window
	if(0 != where)
	{
		win = (void*)where;
		ret = actorinput(win, ev);
	}
	return 0;
}
int actorlist(u8* p)
{
	int j;
	for(j=0;j<0x100;j++)
	{
		if(0 == actor[j].name)break;
		say("[%03x]: %.8s,%.8s\n", j, &actor[j].type, &actor[j].name);
	}
	if(0 == j)say("empty actor\n");
/*
	int j;
	int ret = 0;
	u64 type = 0;
	for(j=0;j<0x100;j++)
	{
		if(actor[j].type != type)
		{
			if(type != 0)say("\n");

			type = actor[j].type;
			if(type == 0)break;

			say("%.*s:\n", 8, &type);
			ret=0;
		}

		if((ret>0)&&(ret%8==0))say("\n");
		say("	%.*s", 8, &actor[j].name);

		ret++;
	}
*/
	return 0;
}
void actorchoose(char* p)
{
	int j,ret;
	u64 name;
	char* q = (char*)&name;
	for(j=0;j<8;j++)
	{
		if(p[j] > 0x20)q[j] = p[j];
	}
	for(;j<8;j++)q[j] = 0;

	for(j=0;j<0x100;j++)
	{
		//all searched
		if(actor[j].name == 0)return;

		//lookat this
		//say("[%s][%s]\n",&actor[j].name, p);
		ret = ncmp(&actor[j].name, p, 8);
		if(ret == 0)
		{
			actor[j].onstart(&actor[j], 0);
			act_at(&arena[0], &actor[j]);
			return;
		}
	}
}
int actorstart(struct arena* win, struct actor* act)
{
	if(act == 0)act = &actor[0];
	act->onstart(act, 0);

	act_at(win, act);
	return 0;
}
int actorstop(struct actor* act)
{
	act->onstop();
	return 0;
}
void actorcreate(u8* addr)
{
	arena = (void*)(addr+0x000000);
	actor = (void*)(addr+0x100000);
	style = (void*)(addr+0x200000);
	compo = (void*)(addr+0x300000);

	//lib1d
	lib1d_create(addr, 0);

	//lib2d
	lib2d_create(addr, 0);

	//lib3d
	lib3d_create(addr, 0);

	//lib4d
	lib4d_create(addr, 0);

	//say("[c,f):createed actor\n");
}
void actordelete()
{
	//say("[c,f):deleteing actor\n");

	lib4d_delete();
	lib3d_delete();
	lib2d_delete();
	lib1d_delete();

	compo = 0;
	style = 0;
	actor = 0;
	arena = 0;
}
