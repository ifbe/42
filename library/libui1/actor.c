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
//
int content_read(void*);
int content_write(void*);
int levitate_read(void*);
int levitate_write(void*);
//
void cli_write(void*);
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

	for(j=0;j<16;j++)
	{
		//
		win = &arena[j];

		//error
		if(win->fmt == 0)break;

		//
		content_read(win);
		if(menu > 0)levitate_read(win);
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
	j = actorstart(0, 0);
	treaty[j].cx = 0x8000;
	treaty[j].cy = 0x8000;
	treaty[j].wantw = 0xffff;
	treaty[j].wanth = 0xffff;

	actor[2].start();
	j = actorstart(0, 2);
	treaty[j].cx = 0x4000;
	treaty[j].cy = 0x4000;
	treaty[j].wantw = 0x8000;
	treaty[j].wanth = 0x8000;

	actor[3].start();
	j = actorstart(0, 3);
	treaty[j].cx = 0xc000;
	treaty[j].cy = 0x4000;
	treaty[j].wantw = 0x8000;
	treaty[j].wanth = 0x8000;

	actor[4].start();
	j = actorstart(0, 4);
	treaty[j].cx = 0x4000;
	treaty[j].cy = 0xc000;
	treaty[j].wantw = 0x8000;
	treaty[j].wanth = 0x8000;

	actor[5].start();
	j = actorstart(0, 5);
	treaty[j].cx = 0xc000;
	treaty[j].cy = 0xc000;
	treaty[j].wantw = 0x8000;
	treaty[j].wanth = 0x8000;

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
