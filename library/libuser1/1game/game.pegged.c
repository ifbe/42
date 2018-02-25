#include "actor.h"
static u8 data[7][7];




static void pegged_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
}
static void pegged_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
	int x,y;
	int cx = sty->i_cx;
	int cy = sty->i_cy;
	int cz = sty->i_cz;
	int ww = sty->i_rx;
	int hh = sty->i_fy;
	int dd = sty->i_uz;
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
static void pegged_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
}
static void pegged_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
}
static void pegged_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
}
static void pegged_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)pegged_read_cli(win, sty, act, com);
	else if(fmt == _tui_)pegged_read_tui(win, sty, act, com);
	else if(fmt == _html_)pegged_read_html(win, sty, act, com);
	else if(fmt == _vbo_)pegged_read_vbo(win, sty, act, com);
	else pegged_read_pixel(win, sty, act, com);
}
static void pegged_write(
	struct actor* act, struct compo* com,
	struct event* ev)
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
static void pegged_delete()
{
}
static void pegged_create()
{
}




void pegged_register(struct actor* p)
{
	p->type = hex32('g', 'a', 'm', 'e');
	p->name = hex64('p', 'e', 'g', 'g', 'e', 'd', 0, 0);
	p->irel = 0;
	p->orel = 0;

	p->oncreate = (void*)pegged_create;
	p->ondelete = (void*)pegged_delete;
	p->onstart  = (void*)pegged_start;
	p->onstop   = (void*)pegged_stop;
	p->onlist   = (void*)pegged_list;
	p->onchoose = (void*)pegged_change;
	p->onread   = (void*)pegged_read;
	p->onwrite  = (void*)pegged_write;
}