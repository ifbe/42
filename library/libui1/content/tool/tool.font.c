#include "actor.h"




static void font_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
	int x,y;
	for(y=2;y<8;y++)
	{
		for(x=0;x<16;x++)
		{
			drawascii(win, x+y*16, 1,
				x*16, y*16, 0xffffffff, 0
			);
		}
	}
}
static void font_read_html(struct arena* win, struct actor* act, struct style* sty)
{
}
static void font_read_vbo(struct arena* win, struct actor* act, struct style* sty)
{
}
static void font_read_tui(struct arena* win, struct actor* act, struct style* sty)
{
}
static void font_read_cli(struct arena* win, struct actor* act, struct style* sty)
{
	say("font(%x,%x,%x)\n",win,act,sty);
}
static void font_read(struct arena* win, struct actor* act, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == __cli__)font_read_cli(win, act, sty);
	else if(fmt == __tui__)font_read_tui(win, act, sty);
	else if(fmt == __html__)font_read_html(win, act, sty);
	else if(fmt == __vbo__)font_read_vbo(win, act, sty);
	else font_read_pixel(win, act, sty);
}
static void font_write(struct event* ev)
{
}
static void font_list()
{
}
static void font_change()
{
}
static void font_start()
{
}
static void font_stop()
{
}
void font_create(void* base, void* addr)
{
	struct actor* p = addr;
	p->type = hex32('t', 'o', 'o', 'l');
	p->name = hex32('f', 'o', 'n', 't');

	p->start = (void*)font_start;
	p->stop = (void*)font_stop;
	p->list = (void*)font_list;
	p->choose = (void*)font_change;
	p->read = (void*)font_read;
	p->write = (void*)font_write;
}
void font_delete()
{
}
