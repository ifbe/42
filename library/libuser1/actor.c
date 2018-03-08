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
void* stylealloc();
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
static struct pinid* pinid = 0;
static int actlen = 0;
static int pinlen = 0;
void* actoralloc()
{
	int j;
	struct actor* act;
	while(1)
	{
		act = &actor[j];
		if(act->type == 0)break;

		j++;
		if(j >= 0x100)return 0;
	}
	return act;
}
void* pinidalloc()
{
	struct pinid* pin = (void*)pinid + pinlen;
	pinlen += sizeof(struct pinid);

	pin->flag00 = 0;
	pin->flag01 = 1;
	pin->flag02 = 2;
	pin->flag03 = 3;

	return pin;
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

	//no window
	if(0 != where)
	{
		win = (void*)where;
		ret = actorinput(win, ev);
	}
	return 0;
}
int actorstart(struct arena* win, struct actor* act)
{
	int w = win->w;
	int h = win->h;
	int d = (w+h) / 2;
	struct style* sty;
	struct pinid* pin;

	sty = stylealloc();
	if(0 == sty)return;

	pin = pinidalloc();
	if(0 == pin)return;

	sty->cx = w/2;
	sty->cy = h/2;
	sty->cz = 0.0;
	sty->rx = 256;
	sty->fy = 256;
	sty->uz = 256;

	relation_write(
		win, sty, _win_,
		act, pin, _act_
	);

	act->onstart(act, pin);
	return 0;
}
int actorstop(struct actor* act)
{
	act->onstop();
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
	return 0;
}
void actorchoose(u8* buf)
{
	//<win0 style="width:50%;height:50%;"> = <2048 pinid="">
	//<win1 style="width:50%;height:50%;"> = <xiangqi pinid="black;expert;">
	//<win2 style="width:49%;height:49%;"> = <weiqi pinid="white;idot;">
	say("@actor:%s\n", buf);
/*
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
*/
}
void actorcreate(u8* addr)
{
	arena = (void*)(addr+0x000000);
	actor = (void*)(addr+0x100000);
	style = (void*)(addr+0x200000);
	pinid = (void*)(addr+0x300000);

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

	pinid = 0;
	style = 0;
	actor = 0;
	arena = 0;
}
