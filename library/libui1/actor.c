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
void* relation_read(u64);
void relation_write(void* uchip, void* ufoot, u64 utype, void* bchip, u64 bfoot, u64 btype);
void arenaread(void*, void*);
void arenawrite(void*, void*);
void login_read(void*);
void login_write(void*, void*);
//
void win_add(u64 why, u64 where);
void win_del(u64 why, u64 where);
void win_at(u64 why, u64 where);
void act_add();
void act_del();
void act_at(void*, void*);
//
int input_write(void*, void*);




static struct arena* arena = 0;
static struct actor* actor = 0;
static struct style* style = 0;




int actorread_one(struct arena* win)
{
	int j;
	struct arena* canvas;
	struct actor* act;		//2048?

	struct relation* rel;		//link
	struct style* st;			//style
	void* pl;

	//cli silent
	if(win->fmt == hex32('c','l','i',0))
	{
		if(win->cw == 12)return 0;
	}

	//canvas
	if(win->type != hex32('b', 'u', 'f', 0))
	{
		canvas = &arena[0];

		canvas->fmt = win->fmt;
		canvas->w = win->w;
		canvas->h = win->h;

		for(j=0;j<16;j++)canvas->info[j] = 0;
	}
	else canvas = win;

	//bg
	background(canvas);

	//content
	rel = win->irel;
	while(1)
	{
		if(rel == 0)break;

		if(rel->selftype == __act__)
		{
			act = (void*)(rel->selfchip);
			st = (void*)(rel->destfoot);
			pl = (void*)(rel->selffoot);
			//say("%x,%x,%x,%x\n", canvas, act, st, pl);
			//say("%x\n", rel);

			act->read(canvas, act, st, pl);
			if(win->cw == 12)
			{
				if(win->fmt == hex32('v','b','o',0))
				{
					select_3d(canvas, st);
				}
				else
				{
					select_2d(canvas, st);
				}
			}
		}
		rel = relation_read(rel->samepinnextchip);
	}

	//fg
	if((win->irel == 0) | (win->cw == 11))login_read(canvas);

theend:
	arenawrite(win, &arena[0]);
	return 0;
}
int actorread()
{
	u64 temp;
	struct arena* canvas;
	struct arena* window;
	struct relation* rel;

	canvas = &arena[0];
	rel = canvas->irel;
	while(1)
	{
		if(rel == 0)break;
		if(rel->destchip == 0)break;

		window = (void*)(rel->selfchip);
		actorread_one(window);

		temp = rel->samepinnextchip;
		rel = relation_read(temp);
	}

	return 0;
}
int actorwrite(struct event* ev)
{
	int ret;
	struct arena* win;
	struct actor* act;
	struct relation* rel;
	//say("%x,%x,%x\n", ev->why, ev->what, ev->where);

	//window event
	if(((ev->what)&0xff) == 'w')
	{
		if(ev->what == hex32('w','+',0,0))win_add(ev->why, ev->where);
		else if(ev->what == hex32('w','-',0,0))win_del(ev->why, ev->where);
		else if(ev->what == hex32('w','@',0,0))win_at(ev->why, ev->where);
		return 0;
	}

	//no window
	if(ev->where < 0xffff)win = &arena[1];
	else win = (void*)(ev->where);

	//pre process
	ret = input_write(win, ev);
	if(ret == 0)return 0;

	//real process
	rel = win->irel;
	while(1)
	{
		if(rel->samepinnextchip == 0)break;
		rel = relation_read(rel->samepinnextchip);
	}
	act = (void*)(rel->selfchip);
	act->write(ev);

	return 0;
}
int actorlist(u8* p)
{
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
			actor[j].start();
			act_at(&arena[1], &actor[j]);
			return;
		}
	}
}
int actorstart(struct arena* win, struct actor* act)
{
	if(act == 0)act = &actor[0];
	act->start();

	act_at(win, act);
	return 0;
}
int actorstop(struct actor* act)
{
	act->stop();
	return 0;
}
void actorcreate(u8* type, u8* addr)
{
	int j;
	if(type != 0)return;
	if( (type == 0)&&(arena != 0) )return;

	//where
	arena = (void*)(addr+0);
	actor = (void*)(addr+0x100000);
	style = (void*)(addr+0x200000);

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

	style = 0;
	actor = 0;
	arena = 0;
}
