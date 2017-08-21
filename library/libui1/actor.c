#include "actor.h"
#define __act__ hex32('a','c','t',0)
#define __win__ hex32('w','i','n',0)
void content_create(void*, void*);
void content_delete();
void helper_create(void*, void*);
void helper_delete();
void lib1d_create(void*, void*);
void lib1d_delete();
void lib2d_create(void*, void*);
void lib2d_delete();
void lib3d_create(void*, void*);
void lib3d_delete();
//
int content_read(void*);
int content_write(void*);
int term_write(void*);
//
void backgroundcolor(void*, u32);
void arenawrite();



static struct arena* arena = 0;
static struct actor* actor = 0;
static struct style* style = 0;
static struct relation* connect = 0;
//
static u32 menu=0;




int actorread()
{
	int j;
	struct arena* window;		//window
	struct arena* canvas;		//buffer
	struct actor* actor;		//2048?

	struct relation* rel;		//link
	struct style* st;			//style

	canvas = &arena[0];
	for(j=1;j<2;j++)
	{
		window = &arena[j];
		if(window->type == 0)break;
		if(window->type == hex32('b', 'u', 'f', 0))continue;

		//cli
		if(window->fmt == 0x696c63)return 0;

		//other
		canvas->fmt = window->fmt;
		canvas->w = window->w;
		canvas->h = window->h;
		rel = window->first;
		while(1)
		{
			if(rel == 0)break;
			else if(rel->destiny == __win__)
			{
				backgroundcolor(canvas, 0xff);
			}
			else if(rel->destiny == __act__)
			{
				//say("%x\n",rel);
				actor = (void*)(rel->chipinfo);
				st = (void*)(rel->footinfo);
				actor->read(canvas, actor, st);
			}
			rel = rel->samepinnextact;
		}

		//send
		arenawrite();
	}//for

	return 0;
}
int actorwrite(struct event* ev)
{
	struct arena* window;
	struct actor* actor;		//2048?
	struct style* st;			//style
	struct relation* rel;		//link
	//say("%x,%x,%x\n", ev->why, ev->what, ev->where);

	//
	if(ev->what == hex32('s','t','r',0))
	{
		term_write((void*)(ev->why));
		term_write("\n");
		return 0;
	}

	//"no window"
	window = (void*)(ev->where);
	if(window == 0)return 0;

	//"cli window"
	if(window->fmt == hex32('c','l','i',0))
	{
		if(ev->what == hex32('c','h','a','r'))
		{
			term_write(&ev[0]);
		}
		return 0;
	}

	//
	rel = window->first;
	if(rel == 0)return 0;
	if(rel->samepinnextact == 0)return 0;
	while(1)
	{
		if(rel->samepinnextact == 0)break;
		rel = rel->samepinnextact;
	}
	if(1)
	{
		actor = (void*)(rel->chipinfo);
		actor->write(ev);
	}
/*
	//prepare
	if(w+)	//new win
	if(w-)	//del win
	if(w@)	//changed memaddr
	if(ev->what == 0x727473)
	{
		cli_write(ev);
		return 0;
	}
	if(ev->where < 0x1000)ev->where = (u64)(&arena[0]);

*/


/*
	//
	int x,y,z,w;
	u64 why,what;
	struct arena* win = (void*)(ev->where);
	struct actor* act;
	struct relation* rel;

	//
	why = ev->why;
	what = ev->what;
	if((what&0xff) == 'p')
	{
		motion_explain(ev);
	}

	//
	why = ev->why;
	what = ev->what;
	if(what == 0x64626b)
	{
		if(why == 0x1b){menu ^= 1;return 0;}
		else if(why == 0xf1){win->dim = 1;return 0;}
		else if(why == 0xf2){win->dim = 2;return 0;}
		else if(why == 0xf3){win->dim = 3;return 0;}
	}

	//
	if(win->dim == 1)
	{
		cli_write(ev);
		return 0;
	}
	else if(win->dim == 2)
	{
		if(menu > 0)
		{
			levitate_write(ev);
			return 0;
		}
		else
		{
			rel = win->top;
			act = rel->child_this;
			act->write(ev);
		}
	}
	else //if(win->dim == 3)
	{
		say("dim=%d\n",win->dim);
	}
*/
	return 0;
}
int actorlist(u8* p)
{
	//列出所有“人物”
	int j;
	int ret;
	u64 type;
	u64 name;
	if(p == 0)
	{
		ret = 0;
		type = 0;
		name = 0;
		for(j=0;j<0x100;j++)
		{
			if(actor[j].name == 0)
			{
				if((ret%8)!=0)say("\n");
				break;
			}

			if(actor[j].type != type)
			{
				say("\n%s:\n",&actor[j].type);
				ret=0;
			}
			
			type = actor[j].type;
			name = actor[j].name;
			if((ret>0)&&(ret%8==0))say("\n");

			say("	%s", &name);
			ret++;
		}
		return 0;
	}

	else
	{
		//start searching
		for(j=0;j<0x100;j++)
		{
			//all searched
			if(actor[j].name == 0)return 0;

			//lookat this
			//say("[%s][%s]\n",&actor[j].name, p);
			ret = ncmp(&actor[j].name, p, 8);
			if(ret == 0)return j;
		}
		return 0;
	}
}
void* actorchoose(u64 pininfo, u64 destiny, u64 chipinfo, u64 footinfo)
{
	int j;
	struct relation* this;
	struct style* st;

	//locate first
	for(j=0;j<0x400;j++)
	{
		if(connect[j].pininfo == 0)
		{
			this = &connect[j];
			break;
		}
	}

	//style
	if(footinfo == 0)footinfo = (u64)&style[j];
	st = (void*)footinfo;
	st->cx = 0x4000 + (getrandom()%0x1000)*8;
	st->cy = 0x4000 + (getrandom()%0x1000)*8;
	st->dim = 2;
	st->wantw = 0x8000;
	st->wanth = 0x8000;

	//generate
	this->pininfo = pininfo;
	this->destiny = destiny;
	this->samepinprevact = 0;
	this->samepinnextact = 0;

	this->chipinfo = chipinfo;
	this->footinfo = footinfo;
	this->samewinprevpin = 0;
	this->samewinnextpin = 0;

	return this;
}
int actorstart(struct arena* win, struct actor* act)
{
	struct relation* winrel;
	struct relation* actrel;
	struct relation* this;

	//win last
	winrel = win->first;
	if(winrel != 0)
	{
		while(1)
		{
			//last rel of win
			if(winrel->samepinnextwin == 0)break;
			winrel = winrel->samepinnextwin;
		}
	}
	else
	{
		winrel = actorchoose(0xff, __win__, (u64)win, 0);
		win->first = winrel;
	}

	//this
	this = actorchoose(0xff, __act__, (u64)act, 0);
	this->samepinprevact = winrel;
	winrel->samepinnextact = this;

	//act last
	actrel = act->first;
	if(actrel != 0)
	{
		while(1)
		{
			//last rel of win
			if(actrel->sameactnextpin == 0)break;
			actrel = actrel->sameactnextpin;
		}

		this->sameactprevpin = actrel;
		actrel->sameactnextpin = this;
	}
	else
	{
		act->start();
	}
}
int actorstop()
{
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
	connect = (void*)(addr+0x300000);

	//lib1d
	lib1d_create(addr, 0);

	//lib2d
	lib2d_create(addr, 0);

	//lib3d
	lib3d_create(addr, 0);

	//
	helper_create(addr, 0);

	//
	content_create(addr, 0);

	//
	actorstart(&arena[1], &actor[1]);
	actorstart(&arena[1], &actor[0]);
	//say("[c,f):createed actor\n");
}
void actordelete()
{
	//say("[c,f):deleteing actor\n");
	content_delete();
	helper_delete();

	lib1d_delete();
	lib2d_delete();
	lib3d_delete();

	connect = 0;
	style = 0;
	actor = 0;
	arena = 0;
}




/*
	actor[0].start();
	j = actorstart(0, 0);
	connect[j].cx = 0x8000;
	connect[j].cy = 0x8000;
	connect[j].wantw = 0xffff;
	connect[j].wanth = 0xffff;

	actor[2].start();
	j = actorstart(0, 2);
	connect[j].cx = 0x4000;
	connect[j].cy = 0x4000;
	connect[j].wantw = 0x8000;
	connect[j].wanth = 0x8000;

	actor[3].start();
	j = actorstart(0, 3);
	connect[j].cx = 0xc000;
	connect[j].cy = 0x4000;
	connect[j].wantw = 0x8000;
	connect[j].wanth = 0x8000;

	actor[21].start();
	j = actorstart(0, 21);
	connect[j].cx = 0x4000;
	connect[j].cy = 0xc000;
	connect[j].wantw = 0x8000;
	connect[j].wanth = 0x8000;
*/
/*
	int t;
	for(t=1;t<50;t++)
	{
		if(connect[t].parent_this == 0)break;
	}
	//
	connect[t].parent_type = 0;
	connect[t].parent_this = &arena[w];
	if(arena[w].bot == 0)	//0
	{
		connect[t].below = 0;
		connect[t].above = 0;

		arena[w].bot = &connect[t];
		arena[w].top = 0;
	}
	else if(arena[w].top == 0)	//1
	{
		(arena[w].bot)->above = &connect[t];

		connect[t].below = arena[w].bot;
		connect[t].above = 0;

		//arena[w].bot = itself
		arena[w].top = &connect[t];
	}
	else	//more
	{
		(arena[w].top)->above = &connect[t];

		connect[t].below = arena[w].top;
		connect[t].above = 0;

		//arena[w].bot = itself
		arena[w].top = &connect[t];
	}

	//
	connect[t].child_type = 0xffffffff;
	connect[t].child_this = &actor[a];
	if(actor[a].first == 0)		//0
	{
		connect[t].prev = 0;
		connect[t].next = 0;

		actor[a].first = &connect[t];
	}
	else if(actor[a].last == 0)		//1
	{
		(actor[a].first)->next = &connect[t];

		connect[t].prev = actor[a].first;
		connect[t].next = 0;

		actor[a].last = &connect[t];
	}
	else	//more
	{
		(actor[a].last)->next = &connect[t];

		connect[t].prev = actor[a].last;
		connect[t].next = 0;

		actor[a].last = &connect[t];
	}

	//
	return t;
*/
/*
	int j,k,ret;
	struct actor* now;

	//exit!
	if( (p==0) | (cmp(p,"exit")==0) )
	{
		say("%x wants to die\n",now);
		eventwrite(0,0,0,0);
		return 0;
	}

	//random
	ret=cmp(p,"random");
	if(ret==0)
	{
		for(j=0;j<10;j++)
		{
			if(actor[j].type != 0)break;	//skip menu|draw
		}
		k=j;

		for(;k<0x100000/0x80;k++)
		{
			if(actor[k].name == 0)break;
		}

		ret = ( getrandom() % (k-j) ) + j;
		now = &actor[ret];
		goto found;
	}

	//search
	ret = actorlist(p);
	if(ret != 0)
	{
		now = &actor[ret];
		goto found;
	}

notfound:
	return 0;

found:
	connect[0].child_this = now;
	now->start();
	return 1;
*/