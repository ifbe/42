#include "arena.h"
#define _cam_ hex32('c','a','m',0)
#define _mic_ hex32('m','i','c',0)
#define _win_ hex32('w','i','n',0)
//
#define _WS_ hex32('W','S',0,0)
#define _ws_ hex32('w','s',0,0)
#define _VNC_ hex32('V','N','C',0)
#define _vnc_ hex32('v','n','c',0)




//cam
void initcam(void*);
void freecam();
int videocreate(void*);
int videodelete(void*);
int videostart(void*);
int videostop(void*);
int videoread(void* win, void* sty, void* act, void* pin);
int videowrite(void*);
int videolist();
int videochoose();
//mic
void initmic(void*);
void freemic();
int soundcreate(void*);
int sounddelete(void*);
int soundstart(void*);
int soundstop(void*);
int soundread(void* win, void* sty, void* act, void* pin);
int soundwrite(void*);
int soundlist();
int soundchoose();
//local
void initwindow(void*);
void freewindow();
int windowcreate(void*);
int windowdelete(void*);
int windowstart(void*);
int windowstop(void*);
int windowread(void* win, void* sty, void* act, void* pin);
int windowwrite(void*);
int windowlist();
int windowchoose();
//remote
void initremote(void*);
void freeremote();
int wsclient_start(void* win, u8* str);
int wsclient_stop(void* win);
int wsclient_read(void* win, void* sty, void* act, void* pin);
int wsclient_write(void* win);
int wsserver_start(void* win);
int wsserver_stop(void* win);
int wsserver_read(void* win, void* sty, void* act, void* pin);
int wsserver_write(void* win);
int vncclient_start(void* win);
int vncclient_stop(void* win);
int vncclient_read(void* win);
int vncclient_write(void* win);
int vncserver_start(void* win);
int vncserver_stop(void* win);
int vncserver_read(void* win);
int vncserver_write(void* win);
//
int parsexml_detail(void*, int, void*, void*, void*, void*);
int ncmp(void*, void*, int);
int cmp(void*, void*);
//
void* samesrcnextdst(void*);
void* samesrcprevdst(void*);
//
void printmemory(void*, int);
void say(void*, ...);




//
static struct arena* arena = 0;
static struct style* style = 0;
static int winlen = 0;
static int stylen = 0;
void* allocarena()
{
	int j;
	for(j=0;j<0x100;j++)
	{
		if(0 == arena[j].type)break;
	}
	if(j >= 0x100)return 0;
	return &arena[j];
}
void* allocstyle()
{
	int j;
	u8* sty = (void*)style + stylen;
	stylen += sizeof(struct style);

	for(j=0;j<sizeof(struct style);j++)sty[j] = 0;
	return sty;
}




int arenadelete(struct arena* win)
{
	if(win == 0)return 0;
	windowstop(win);

	win->type = 0;
	win->fmt = 0;
	win->irel = 0;
	win->orel = 0;
	return 0;
}
void* arenacreate(u64 type, u64 addr)
{
	int j = 0;
	struct arena* win = allocarena();
	if(0 == win)return 0;

	if(_win_ == type)
	{
		win->type = _win_;
		win->fmt = 0;

		windowstart(win);
	}
	else if(_cam_ == type)
	{
		if(0 == addr)return 0;
		say("cam://%s\n", (void*)addr);

		win->type = _cam_;
		win->fmt = hex32('y','u','v',0);
		videostart(win);
	}
	else if(_mic_ == type)
	{
		if(0 == addr)return 0;
		say("mic://%s\n", (void*)addr);

		win->type = _mic_;
		win->fmt = hex32('p','c','m',0);
		soundstart(win);
	}
	else if(_ws_ == type)
	{
		if(0 == addr)return 0;
		say("ws://%s\n", (void*)addr);

		win->type = _ws_;
		win->fmt = hex32('d','a','t','a');

		//be client, accept data
		wsclient_start(win, (void*)addr);
	}
	else if(_WS_ == type)
	{
		win->type = _WS_;
		win->fmt = hex32('h','t','m','l');

		//be server, output data
		win->fd = addr;
		wsserver_start(win);
	}
/*
	else if(_vnc_ == type)
	{
		win->type = _vnc_;
		win->fmt = hex32('d','a','t','a');
		win->irel = 0;
		win->orel = 0;

		//be client, accept data
		vncclient_start(win);
	}
	else if(_VNC_ == type)
	{
		win->type = _VNC_;
		win->fmt = 0;
		win->irel = 0;
		win->orel = 0;

		//be server, output data
		win->fd = fd;
		vncserver_start(win);
	}
*/
	win->irel = 0;
	win->orel = 0;
	return win;
}
int arenastop()
{
	return 0;
}
int arenastart()
{
	return 0;
}
int arenaread()
{
	int j;
	struct relation* rel;
	struct arena* win;
	struct actor* act;
	struct style* sty;
	struct pinid* pin;

	for(j=0;j<16;j++)
	{
		win = &arena[j];
		if(0 == win->type)break;

		if(_win_ == win->type)
		{
			windowwrite(win);
		}
		else if(_cam_ == win->type)
		{
			rel = win->orel;
			while(1)
			{
				if(0 == rel)break;
				act = (void*)(rel->destchip);
				pin = (void*)(rel->destfoot);
				sty = (void*)(rel->selffoot);
				videoread(win, sty, act, pin);
				rel = samesrcnextdst(rel);
			}
		}
		else if(_mic_ == win->type)
		{
			rel = win->orel;
			while(1)
			{
				if(0 == rel)break;
				act = (void*)(rel->destchip);
				pin = (void*)(rel->destfoot);
				sty = (void*)(rel->selffoot);
				soundread(win, sty, act, pin);
				rel = samesrcnextdst(rel);
			}
		}
		else if(_ws_ == win->type)
		{
			rel = win->orel;
			while(1)
			{
				if(0 == rel)break;
				act = (void*)(rel->destchip);
				pin = (void*)(rel->destfoot);
				sty = (void*)(rel->selffoot);
				wsclient_read(win, sty, act, pin);
				rel = samesrcnextdst(rel);
			}
		}
		else if((_WS_ == win->type)|(0 != win->dirty))
		{
			wsserver_write(win);
			win->dirty = 0;
		}
	}
	return 0;
}
int arenawrite(struct event* ev)
{
	int j;
	void* ret;
	u64 why = ev->why;
	u64 what = ev->what;
	u64 where = ev->where;
	//say("@arenawrite:%llx,%llx,%llx\n", why, what, where);

	if(hex32('w','+',0,0) == what)
	{
		ret = arenacreate(why, where);
		if(ret == 0)
		{
			say("error@w+\n");
			return 0;
		}
	}
	else if(hex32('w','-',0,0) == what)
	{
		ret = (void*)where;
		arenadelete(ret);
	}
	else
	{
		if(where >= 0x1000)return 0;

		for(j=0;j<0x100;j++)
		{
			if((_WS_ == arena[j].type)&&(where == arena[j].fd))
			{
				wsserver_read(&arena[j], 0, 0, 0);
			}
		}
	}
	return 0;
}
void* arenalist(u8* buf, int len)
{
	int j;
	if(0 == buf)
	{
		for(j=0;j<0x100;j++)
		{
			if(0 == arena[j].type)break;
			say("[%03x]: %.8s,%.8s\n", j, &arena[j].type, &arena[j].fmt);
		}
		if(0 == j)say("empty arena\n");
	}
	else
	{
		for(j=0;j<len;j++){if('@' == buf[j])break;}

		j = buf[j+1]-0x30;
		if(j >= 10)j = 0;

		if(0 == arena[j].type)return 0;
		return &arena[j];
	}
	return 0;
}
void* arenachoose(u8* buf, int len)
{
	u64 name = 0;
	int id = 0;
	u8* data = 0;
	int dl = 0;

	parsexml_detail(buf, len, &name, &id, &data, &dl);
	say("%.*s\n", len, buf);
	say("%llx, %x\n", name, id);
	say("%.*s\n", dl, data);

	if(_win_ == name)
	{
		if((id>0)&&(id<0x1000))
		{
			if(0 == arena[id].type)
			{
				arenacreate(_win_, 0);
			}
		}
	}

	return 0;
}





void freearena()
{
	//say("[c,f):freeing arena\n");

	freecam();
	freemic();
	freewindow();
	//remotedelete();
}
void initarena(u8* addr)
{
	int j;
	for(j=0;j<0x400000;j++)addr[j] = 0;

	arena = (void*)(addr+0x000000);
	style = (void*)(addr+0x200000);

	//remotecreate(arena);
	initwindow(arena);
	initmic(arena);
	initcam(arena);

	arenacreate(_win_, 0);

	//say("[c,f):inited arena\n");
}
