#include "actor.h"
void content_create(void*, void*);
void content_delete();
void levitate_create(void*, void*);
void levitate_delete();
void lib1d_create(void*, void*);
void lib1d_delete();
void lib2d_create(void*, void*);
void lib2d_delete();
void lib3d_create(void*, void*);
void lib3d_delete();
//2d
int levitate_read(void*);
int levitate_write(void*);
//
void vt100_read(void*, int, int, int, int);
void cli_write(void*);
//
void motion_explain(void*);



static struct arena* arena = 0;
static struct actor* actor = 0;
static struct relation* treaty = 0;
//
static u32 menu=0;




int actorread()
{
	int j,k;
	struct arena* win;
	struct relation* rel;
	struct actor* act;
	for(j=0;j<16;j++)
	{
		//
		win = &arena[j];

		//error
		if(win->fmt == 0)break;

		//title
		//if(win->fmt == 0x)title_read(win);

		//tray
		//if(win->fmt == 0x)tray_read(win);

		//voice
		if(win->fmt == 0x6563696f76)continue;




		//1d:	cli
		if(win->dim == 1)
		{
			if(win->fmt == 0x696c63)return 0;

			vt100_read(win, 0, 0, 0xffff, 0xffff);
			return 0;
		}

		//2d:	rgba
		else if(win->dim == 2)
		{
			rel = win->bot;
			for(k=0;k<16;k++)
			{
				if(rel == 0)break;

				act = rel->child_this;
				act->read(rel);

				rel = rel->above;
			}

			//
			if(menu > 0)levitate_read(win);
			return 0;
		}

		//3d:	directx, opengl, vulkan
		else if(win->dim == 3)
		{
			return 0;
		}//dim=3
	}//for
	return 0;
}
int actorwrite(struct event* ev)
{
	//prepare
/*
	if(w+)	//new win
	if(w-)	//del win
	if(w@)	//changed memaddr
*/
	if(ev->what == 0x727473)
	{
		cli_write(ev);
		return 0;
	}
	if(ev->where < 0x1000)ev->where = (u64)(&arena[0]);




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
int actorchoose(u8* p)
{
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
	treaty[0].child_this = now;
	now->start();
	return 1;
}
int actorstart(int w, int a)
{
	int t;
	for(t=1;t<50;t++)
	{
		if(treaty[t].parent_this == 0)break;
	}
	treaty[t].cx = 0x4000 + 0x8000*(a%2);
	treaty[t].cy = 0x4000 + 0x8000*((a/2)%2);
	treaty[t].wantw = 0x8000;
	treaty[t].wanth = 0x8000;

	//
	treaty[t].parent_type = 0;
	treaty[t].parent_this = &arena[w];
	if(arena[w].bot == 0)	//0
	{
		treaty[t].below = 0;
		treaty[t].above = 0;

		arena[w].bot = &treaty[t];
		arena[w].top = 0;
	}
	else if(arena[w].top == 0)	//1
	{
		(arena[w].bot)->above = &treaty[t];

		treaty[t].below = arena[w].bot;
		treaty[t].above = 0;

		//arena[w].bot = itself
		arena[w].top = &treaty[t];
	}
	else	//more
	{
		(arena[w].top)->above = &treaty[t];

		treaty[t].below = arena[w].top;
		treaty[t].above = 0;

		//arena[w].bot = itself
		arena[w].top = &treaty[t];
	}

	//
	treaty[t].child_type = 0xffffffff;
	treaty[t].child_this = &actor[a];
	if(actor[a].first == 0)		//0
	{
		treaty[t].prev = 0;
		treaty[t].next = 0;

		actor[a].first = &treaty[t];
	}
	else if(actor[a].last == 0)		//1
	{
		(actor[a].first)->next = &treaty[t];

		treaty[t].prev = actor[a].first;
		treaty[t].next = 0;

		actor[a].last = &treaty[t];
	}
	else	//more
	{
		(actor[a].last)->next = &treaty[t];

		treaty[t].prev = actor[a].last;
		treaty[t].next = 0;

		actor[a].last = &treaty[t];
	}

	//
	return t;
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

	//clean [1m,4m)
	for(j=0x100000;j<0x400000;j++)addr[j] = 0;

	//where
	arena = (void*)(addr+0);
	actor = (void*)(addr+0x100000);
	treaty = (void*)(addr+0x200000);

	//lib1d
	lib1d_create(addr, 0);

	//lib2d
	lib2d_create(addr, 0);

	//lib3d
	lib3d_create(addr, 0);

	//bottom
	content_create(addr, 0);

	//upper
	levitate_create(addr, 0);

	//
	actor[0].start();
	actorstart(0, 0);
	actor[2].start();
	actorstart(0, 2);
	actor[3].start();
	actorstart(0, 3);
	actor[4].start();
	actorstart(0, 4);
	actor[5].start();
	actorstart(0, 5);
	//say("[c,f):createed actor\n");
}
void actordelete()
{
	//say("[c,f):deleteing actor\n");
	content_delete();

	lib1d_delete();
	lib2d_delete();
	lib3d_delete();

	treaty = 0;
	actor = 0;
	arena = 0;
}
