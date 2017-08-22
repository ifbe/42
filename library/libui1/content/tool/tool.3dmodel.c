#include "actor.h"
void background1(void*);




static void stl_read_html(struct arena* win)
{
}
static void stl_read_text(struct arena* win)
{
}
static void stl_read_stl(struct arena* win)
{
	int j;
	char* buf = (void*)(win->buf);
	readfile("42.stl", buf, 0, 0x800000);
}
static void stl_read(struct arena* win)
{
	u64 fmt = win->fmt;

	//text
	if(fmt == 0x74786574)
	{
		stl_read_text(win);
	}

	//html
	else if(fmt == 0x6c6d7468)
	{
		stl_read_html(win);
	}

	//pixel
	else if(fmt == hex32('s','t','l',0))
	{
		stl_read_stl(win);
	}
}




static void stl_write(struct event* ev)
{
	u64 type = ev->what;
	u64 key = ev->why;

	if(type == 0x2d70)
	{
		int x = key&0xffff;
		int y = (key>>16)&0xffff;
	}
	if(type == 0x72616863)
	{
	}
}




static void stl_list()
{
}
static void stl_change()
{
}
static void stl_start()
{
}
static void stl_stop()
{
}
void stl_create(void* base,void* addr)
{
	struct actor* p = addr;
	p->type = hex32('t', 'o', 'o', 'l');
	p->name = hex32('s', 't', 'l', 0);

	p->start = (void*)stl_start;
	p->stop = (void*)stl_stop;
	p->list = (void*)stl_list;
	p->choose = (void*)stl_change;
	p->read = (void*)stl_read;
	p->write = (void*)stl_write;
}
void stl_delete()
{
}
