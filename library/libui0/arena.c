#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#include "arena.h"





//voice
int voicecreate(void*);
int voicedelete();
int voicestart(int);
int voicestop();
int voicelist();
int voicechoose();
int voiceread();
int voicewrite(void*);
//network
int netwincreate(void*);
int netwindelete();
int netwinstart(int);
int netwinstop();
int netwinlist();
int netwinchoose();
int netwinread();
int netwinwrite(void*);
//local
int windowcreate(void*);
int windowdelete();
int windowstart(void*);
int windowstop();
int windowlist();
int windowchoose();
int windowread();
int windowwrite(void* dst, void* src);
//
int ncmp(void*, void*, int);
int cmp(void*, void*);
//
void* startmemory(int);
int stopmemory(void*);
//
void printmemory(void*, int);
void say(void*, ...);




//
static struct window* arena;




int arenaread()
{
	return 0;
}
int arenawrite()
{
	windowwrite(&arena[1], &arena[0]);
}
/*
u64 arenalist(u64 dispid, u64 property)
{
	int j;
	if(ncmp("buffer", &property, 8) == 0)return arena[dispid].buf;
	else if(ncmp("format", &property, 8) == 0)return arena[dispid].fmt;
	else if(ncmp("width" , &property, 8) == 0)return arena[dispid].w;
	else if(ncmp("height", &property, 8) == 0)return arena[dispid].h;

	//if(property==unknown)
	for(j=0;j<10;j++)
	{
		if(arena[j].fmt == 0)break;

		say(
		"%llx,%llx,%llx,%llx\n"
		"%llx,%llx,%llx,%llx\n",
		arena[j].type,
		arena[j].fmt,
		arena[j].buf,
		arena[j].len,
		arena[j].w,
		arena[j].h,
		arena[j].d,
		arena[j].dim
		);
	}
	return 0;
}
u64 arenachoose(u64 dispid, u64 property, u64 what)
{
	if(ncmp("fmt", &property, 8) == 0)arena[dispid].fmt = what;
	else if(ncmp("buf", &property, 8) == 0)arena[dispid].buf = what;
	else if(ncmp("width" , &property, 8) == 0)arena[dispid].w = what;
	else if(ncmp("height", &property, 8) == 0)arena[dispid].h = what;
	else what = 0;	//fail

	return what;
}
*/
int arenastart(struct window* win)
{
	win->first = 0;
	win->last = 0;

	if(win->type == hex32('n', 'e', 't', 0))
	{
	}
	else
	{
		//win: create window
		//buf: register bmp&ximage
		windowstart(win);
	}
	return 0;
}
int arenastop()
{
	return 0;
}
void arenacreate(u8* type, u8* addr)
{
	int j;
	if(type != 0)return;
	for(j=0;j<0x400000;j++)addr[j] = 0;

	//create
	arena = (void*)addr;
	windowcreate(arena);
	voicecreate(arena);

	//start
	arena[0].type = hex32('b', 'u', 'f', 0);
	arena[0].fmt = hex32('a', 'n', 'y', 0);
	arena[0].buf = (u64)startmemory(0x100000*16);
	arena[0].len = 0x100000*16;
	arenastart(&arena[0]);
	arenastart(&arena[1]);

	//say("[c,f):createed arena\n");
}
void arenadelete()
{
	//
	//say("[c,f):deleteing arena\n");

	//1024*1024*4
	windowdelete();
}
