#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#include "arena.h"





//voice
int voicecreate(void*);
int voicedelete();
int voicestart(void*);
int voicestop();
int voicelist();
int voicechoose();
int voiceread();
int voicewrite(void*);
//network
int netwincreate(void*);
int netwindelete();
int netwinstart(void*);
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
int websocket_write(int fd, void* buf, int len);
//
int ncmp(void*, void*, int);
int cmp(void*, void*);
//
void* startmemory(int);
int stopmemory(void*);
//
void* connect_read(int);
void connect_write(void*,u64,u64, void*,u64,u64);
//
void printmemory(void*, int);
void say(void*, ...);




//
static struct window* arena;




int arenaread(struct window* dst, struct window* src)
{
	return 0;
}
int arenawrite(struct window* dst, struct window* src)
{
	void* buf;
	if(dst->type == hex32('w','i','n',0))
	{
		windowwrite(dst, &arena[0]);
	}
	else if(dst->type == hex32('W','S', 0, 0))
	{
		websocket_write(dst->fd, src->buf, src->info[0]);
	}
}
int arenastart(u64 type, int fd)
{
	struct window* win;
	int j;
	if(type == hex32('b', 'u', 'f', 0))
	{
		if(arena->type == 0)
		{
			arena->type = hex32('b', 'u', 'f', 0);
			arena->fmt = hex32('a', 'n', 'y', 0);
			arena->first = 0;
			arena->last = 0;

			arena->buf = startmemory(0x100000*16);
			arena->len = 0x100000*16;
			windowstart(arena);
		}
		return 0;
	}
	else
	{
		j = 1;
		while(1)
		{
			win = &arena[j];
			if(win->type == 0)break;

			j++;
			if(j >= 0x100)return -1;
		}

		if(type == hex32('w', 'i', 'n', 0))
		{
			win->type = hex32('w', 'i', 'n', 0);
			win->fmt = 0;
			win->first = 0;
			win->last = 0;

			windowstart(win);
			connect_write(arena, 0, 0, win, 0, 0);
			return j;
		}
		else if(type == hex32('w','s',0,0))
		{
			win->type = hex32('w', 's', 0, 0);
			win->fmt = hex32('h','t','m','l');
			win->first = 0;
			win->last = 0;

			win->fd = fd;
			connect_write(arena, 0, 0, win, 0, 0);
			return j;
		}
	}
	return 0;
}
int arenastop(struct window* win)
{
	if(win == 0)return 0;

	win->type = 0;
	win->fmt = 0;
	win->first = 0;
	win->last = 0;
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

	//
	arenastart(hex32('b', 'u', 'f', 0), 0);
	arenastart(hex32('w', 'i', 'n', 0), 0);

	//say("[c,f):createed arena\n");
}
void arenadelete()
{
	//
	//say("[c,f):deleteing arena\n");

	//1024*1024*4
	windowdelete();
}
