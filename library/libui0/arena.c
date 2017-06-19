#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#include "arena.h"





//voice
int voicecreate();
int voicedelete();
int voicestart(int);
int voicestop();
int voicelist();
int voicechoose();
int voiceread();
int voicewrite(void*);
//network
int netwincreate();
int netwindelete();
int netwinstart(int);
int netwinstop();
int netwinlist();
int netwinchoose();
int netwinread();
int netwinwrite(void*);
//local
int windowcreate();
int windowdelete();
int windowstart(void*);
int windowstop();
int windowlist();
int windowchoose();
int windowread();
int windowwrite(void*);
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
static int now = 0;




u64 arenaread()
{
	return 0;
}
u64 arenawrite()
{
	int j;
	for(j=0;j<16;j++)
	{
		if(arena[j].fmt == 0)break;
		if(arena[j].fmt == 0x696c63)continue;
		if(arena[j].fmt == 0x6563696f76)continue;

		if(arena[j].fmt == 0x6c6d7468)netwinwrite(&arena[j]);
		else windowwrite(&arena[j]);
	}
	return 0;
}
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
int arenastart(int type)
{
	if(type == 0)
	{
		arena[now].type = 0;
		arena[now].fmt = 0;
		arena[now].bot = 0;
		arena[now].top = 0;

		arena[now].buf = 0;
		arena[now].len = 0;
		arena[now].dim = 2;

		arena[now].w = 512;
		arena[now].h = 512;
		windowstart( &(arena[now]) );
		return now;
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
	if(type!=0)return;

	//table where
	arena = (void*)addr;
	for(j=0;j<0x100000;j++)addr[j] = 0;

	//local
	windowcreate();
	voicecreate();

	//
	//default window
	arenastart(0);
	now = 0;

	//say("[c,f):createed arena\n");
}
void arenadelete()
{
	//
	//say("[c,f):deleteing arena\n");

	//1024*1024*4
	windowdelete();
}
