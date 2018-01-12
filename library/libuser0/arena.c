#include "arena.h"
#define __buf__ hex32('b','u','f',0)
#define __win__ hex32('w','i','n',0)





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
void* relation_read(int);
void relation_write(void*,u64,u64, void*,u64,u64);
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
	if(dst->type == __win__)
	{
		windowwrite(dst, &arena[0]);
	}
	else if(dst->type == hex32('W','S', 0, 0))
	{
		websocket_write(dst->fd, src->buf, src->info[0]);
	}
	return 0;
}
void* arenastart(u64 type, u64 fd)
{
	struct window* win;
	int j;
	if(type == __buf__)
	{
		if(arena->type == 0)
		{
			arena->type = __buf__;
			arena->fmt = __buf__;
			arena->first = 0;
			arena->last = 0;

			arena->buf = startmemory(0x100000*16);
			arena->len = 0x100000*16;
			windowstart(arena);
		}
		return arena;
	}
	else
	{
		j = 1;
		while(1)
		{
			win = &arena[j];
			if(win->type == 0)break;

			j++;
			if(j >= 0x100)return 0;
		}

		if(type == __win__)
		{
			win->type = __win__;
			win->fmt = 0;
			win->first = 0;
			win->last = 0;

			windowstart(win);
			relation_write(
				arena, 0, __buf__,
				win, 0, __win__
			);
			return win;
		}
		else if(type == hex32('W','S',0,0))
		{
			win->type = hex32('W', 'S', 0, 0);
			win->fmt = hex32('h','t','m','l');
			win->first = 0;
			win->last = 0;

			win->fd = fd;
			relation_write(
				arena, 0, 0,
				win, 0, 0
			);
			return win;
		}
	}
	return 0;
}
int arenastop(struct window* win)
{
	if(win == 0)return 0;
	windowstop(win);

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
