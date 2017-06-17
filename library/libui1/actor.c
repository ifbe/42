#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#include "actor.h"




//
int cli_read(void*);
int cli_write(void*);
void draw_vt100(void*, int, int, int, int);
void carve_term3d(void*);
//
void content_create(void*, void*);
void content_delete();
void external_create(void*, void*);
void external_delete();
//
void lib1d_create(void*, void*);
void lib1d_delete();
void lib2d_create(void*, void*);
void lib2d_delete();
void lib3d_create(void*, void*);
void lib3d_delete();
//
int ncmp(void*,void*,int);
int cmp(void*,void*);
u32 getrandom();
//
void eventwrite(u64,u64,u64,u64);
void say(void*, ...);




static struct window* arena = 0;
static struct working* actor = 0;
static struct relation* treaty = 0;
//
static u32 menu=0;
static u32 newline=1;




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
	u64 now = treaty[0].child_id;

	//exit!
	if( (p==0) | (cmp(p,"exit")==0) )
	{
		say("%d wants to die\n",now);
		eventwrite(0,0,0,0);
		return 0;
	}

	//next
	ret=cmp(p,"+");
	if(ret==0)
	{
		if(actor[now+1].name == 0)return 0;
		now++;
		goto found;
	}

	//last
	ret=cmp(p,"-");
	if(ret==0)
	{
		if(actor[now-1].type == 0)return 0;
		now--;
		goto found;
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

		now=( getrandom() % (k-j) ) + j;
		goto found;
	}

	//search
	ret = actorlist(p);
	if(ret != 0)
	{
		now = ret;
		goto found;
	}

notfound:
	return 0;

found:
	treaty[0].child_id = now;
	actor[now].start();
	return now;
}
int actorstart()
{
	int this = treaty[0].child_id;
	return actor[this].start();
}
int actorread()
{
	int j;
	int now;
	struct window* w;
	struct working* p;
	struct relation* t;
	for(j=0;j<16;j++)
	{
		//
		w = &arena[j];
		p = &actor[now];
		t = &treaty[0];
		now = t->child_id;

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
				actor[now].read(w, p, t);
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
			actor[now].read(w, p, t);

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
	int ret;
/*
	if(w+)	//new win
	if(w-)	//del win
	if(w@)	//changed memaddr
*/
	if(ev[1] == 0x727473)
	{
		ret = actorchoose( (void*)(ev[0]) );
		if(ret == 0)say("%s\n", ev[0]);
		return 0;
	}
	if(ev[2] < 0x1000)ev[2] = (u64)(&arena[2]);




	//process
	u64 why = ev[0];
	u64 what = ev[1];
	struct window* w = (void*)ev[2];
	//u64 when = ev[3];

	//kbd
	if(what == 0x64626b)
	{
		if(why == 0xf1){w->dim = 1;return 0;}
		else if(why == 0xf2){w->dim = 2;return 0;}
		else if(why == 0xf3){w->dim = 3;return 0;}
	}//kbd

	//
	if(w->dim == 1)
	{
		if(what == 0x64626b)
		{
			ev[1] = 0x72616863;
			if(why == 0x1b)ev[0] = 0x1b;
		}

		newline = cli_write(ev);
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
		return actor[treaty[0].child_id].write(ev);
	}
	else if(w->dim == 3)
	{
	}

	say("dim=%d\n",w->dim);
	return 0;
}
int actorstop()
{
	int this = treaty[0].child_id;
	return actor[this].stop();
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

	//
	treaty[0].parent_id = 2;
	treaty[0].child_id = 2;
	treaty[0].cx = 0x8000;
	treaty[0].cy = 0x8000;
	treaty[0].width = 0xe000;
	treaty[0].height = 0xe000;
	//say("[c,f):createed actor\n");
}
void actordelete()
{
	//say("[c,f):deleteing actor\n");

	content_delete();
	external_delete();

	lib1d_delete();
	lib2d_delete();
	lib3d_delete();

	treaty = 0;
	actor = 0;
	arena = 0;
}
