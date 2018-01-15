#include "actor.h"
static u8 data[7][7];




static void pegged_read_vbo(struct arena* win, struct actor* act, struct style* sty)
{
}
static void pegged_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
	int x,y;
	int cx = (sty->cx) * (win->w) / 0x10000;
	int cy = (sty->cy) * (win->h) / 0x10000;
	int ww = (sty->wantw) * (win->w) / 0x20000;
	int hh = (sty->wanth) * (win->h) / 0x20000;
	drawline_rect(win, 0xff00, cx-ww, cy-hh, cx+ww, cy+hh);

	for(y=0;y<7;y++)
	{
		for(x=0;x<7;x++)
		{
			if(data[y][x] == 0)continue;
			drawsolid_circle(
				win, 0xffffff,
				cx+(x-3)*ww/4,
				cy+(y-3)*hh/4,
				ww/32
			);
		}
	}
}
static void pegged_read_html(struct arena* win, struct actor* act, struct style* sty)
{
}
static void pegged_read_tui(struct arena* win, struct actor* act, struct style* sty)
{
}
static void pegged_read_cli(struct arena* win, struct actor* act, struct style* sty)
{
}
static void pegged_read(struct arena* win, struct actor* act, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == __cli__)pegged_read_cli(win, act, sty);
	else if(fmt == __tui__)pegged_read_tui(win, act, sty);
	else if(fmt == __html__)pegged_read_html(win, act, sty);
	else if(fmt == __vbo__)pegged_read_vbo(win, act, sty);
	else pegged_read_pixel(win, act, sty);
}
static void pegged_write(struct event* ev)
{
}




static void pegged_list()
{
}
static void pegged_change()
{
}
static void pegged_start()
{
	int x,y;
	for(y=0;y<7;y++)
	{
		for(x=0;x<7;x++)
		{
			if((x>=2)&&(x<=4))data[y][x] = 1;
			else if((y>=2)&&(y<=4))data[y][x] = 1;
			else data[y][x] = 0;
		}
	}
}
static void pegged_stop()
{
}
void pegged_create(void* base,void* addr)
{
	struct actor* p = addr;
	p->type = hex32('g', 'a', 'm', 'e');
	p->name = hex64('p', 'e', 'g', 'g', 'e', 'd', 0, 0);

	p->start = (void*)pegged_start;
	p->stop = (void*)pegged_stop;
	p->list = (void*)pegged_list;
	p->choose = (void*)pegged_change;
	p->read = (void*)pegged_read;
	p->write = (void*)pegged_write;
}
void pegged_delete()
{
}
