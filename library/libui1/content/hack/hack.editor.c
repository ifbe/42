#include "actor.h"




static void editor_read_text(struct arena* win)
{
}
static void editor_read_html(struct arena* win)
{
}
static void editor_read_pixel(struct arena* win)
{
}
static void editor_read(struct arena* win)
{
	u64 fmt = win->fmt;

	//text
	if(fmt == 0x74786574)
	{
		editor_read_text(win);
	}

	//html
	else if(fmt == 0x6c6d7468)
	{
		editor_read_html(win);
	}

	//pixel
	else
	{
		editor_read_pixel(win);
	}
}
static void editor_write(struct event* ev)
{
}
static void editor_list()
{
}
static void editor_choose()
{
}
static void editor_start()
{
}
static void editor_stop()
{
}
void editor_create(void* base,void* addr)
{
	struct actor* p = addr;
	p->type = hex32('t', 'o', 'o', 'l');
	p->name = hex64('e', 'd', 'i', 't', 'o', 'r', 0, 0);

	p->start = (void*)editor_start;
	p->stop = (void*)editor_stop;
	p->list = (void*)editor_list;
	p->choose = (void*)editor_choose;
	p->read = (void*)editor_read;
	p->write = (void*)editor_write;
}
void editor_delete()
{
}
