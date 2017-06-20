#include "actor.h"




int cli_read(void*);
int cli_write(void*);
void draw_vt100(void*, int, int, int, int);



static struct arena* arena = 0;
static struct actor* actor = 0;
static struct relation* treaty = 0;
//
static u64 tmp=0;
static u32 menu=0;
static u32 newline=1;




int actorread()
{
	int j;
	struct arena* w;
	struct actor* p;
	struct relation* t;
	for(j=0;j<16;j++)
	{
		//
		w = &arena[j];
		t = &treaty[0];
		p = t->child_this;

		//error
		if(w->fmt == 0)break;

		//title
		//if(w->fmt == 0x)title_read(w);

		//tray
		//if(w->fmt == 0x)tray_read(w);

		//voice
		if(w->fmt == 0x6563696f76)continue;

		//1d:	cli
		if(w->dim == 1)
		{
			if(newline == 1)
			{
				//actor[now].read(w, p, t);
				cli_read(w);
				newline = 0;
			}
			if(w->fmt == 0x696c63)return 0;

			draw_vt100(w, 0x800, 0x800, 0xf800, 0xf800);
			return 0;
		}

		//2d:	rgba
		else if(w->dim == 2)
		{
			p->read(w, p, t);

			if(menu > 0)
			{
				actor[1].read(w, p, t);
				actor[0].read(w, p, t);
			}
			return 0;
		}

		//3d:	directx, opengl, vulkan
		else if(w->dim == 3)
		{
/*
			if(w->dim == 1)
			{
				carve_term3d(w);
			}
			else if(w->dim == 2)
			{
				carve_texture(w);
			}
			else actor[now].read(w, p, t);

			if(menu > 0)
			{
				actor[1].read(w, p, t);
				actor[0].read(w, p, t);
			}
*/
			return 0;
		}//dim=3
	}//for
	return 0;
}
int actorwrite(u64* ev)
{
	//prepare
	int x,y;
/*
	if(w+)	//new win
	if(w-)	//del win
	if(w@)	//changed memaddr
*/
	if(ev[1] == 0x727473)
	{
		cli_write((void*)ev[0]);
		return 0;
	}
	if(ev[2] < 0x1000)ev[2] = (u64)(&arena[0]);




	//process
	u64 why = ev[0];
	u64 what = ev[1];
	struct arena* w = (void*)ev[2];
	struct relation* t = treaty;
	struct actor* p = treaty->child_this;
	//u64 when = ev[3];

	//kbd
	if(what == 0x64626b)
	{
		if(why == 0xf1){w->dim = 1;return 0;}
		else if(why == 0xf2){w->dim = 2;return 0;}
		else if(why == 0xf3){w->dim = 3;return 0;}
	}//kbd

	//mouse
	if((what&0xff)=='p')
	{
		if((why>>48)=='r')
		{
			if(what == 0x2b70)tmp = why;
			else if(what == 0x2d70)tmp = 0;
			else if(what == 0x4070)
			{
				if(tmp != 0)
				{
					x = (why&0xffff) - (tmp&0xffff);
					y = ((why>>16)&0xffff) - ((tmp>>16)&0xffff);
					t->cx += x;
					t->cy += y;
					tmp = why;
				}
			}
		}
		else if((why>>48)=='b')
		{
			t->width = treaty[0].width*90/100;
			t->height = treaty[0].height*90/100;
		}
		else if((why>>48)=='f')
		{
			t->width = treaty[0].width*110/100;
			t->height = treaty[0].height*110/100;
		}
	}//mouse

	//
	if(w->dim == 1)
	{
		newline = cli_write(&ev[0]);
		return 0;
	}
	else if(w->dim == 2)
	{
		if( (what == 0x64626b)&&(why == 0x1b) )
		{
			menu ^= 1;
			return 0;
		}

		if(menu > 0)
		{
			actor[1].write(ev);
			actor[0].write(ev);
		}
		return p->write(ev);
	}
	else //if(w->dim == 3)
	{
		say("dim=%d\n",w->dim);
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
int actorstart(int win, int wk)
{
	//
	treaty[0].parent_type = 0;
	treaty[0].parent_this = &arena[0];
	treaty[0].parent_prev = 0;
	treaty[0].parent_next = 0;

	treaty[0].child_type = 1;
	treaty[0].child_this = &actor[2];
	treaty[0].child_below = 0;
	treaty[0].child_above = 0;

	treaty[0].cx = 0x8000;
	treaty[0].cy = 0x8000;
	treaty[0].width = 0xe000;
	treaty[0].height = 0xe000;

	actor[2].start();
	return 0;
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

	//special
	external_create(addr, 0);

	//ordinary
	content_create(addr, 0);

	actorstart(0,2);
	//say("[c,f):createed actor\n");
}
void actordelete()
{
	//say("[c,f):deleteing actor\n");
	actorstop();

	content_delete();
	external_delete();

	lib1d_delete();
	lib2d_delete();
	lib3d_delete();

	treaty = 0;
	actor = 0;
	arena = 0;
}
