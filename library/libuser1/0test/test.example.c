#include "actor.h"
u32 getrandom();




static void example_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
}
static void example_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
	u32 bg,fg;
	int cx = sty->i_cx;
	int cy = sty->i_cy;
	int cz = sty->i_cz;
	int ww = sty->i_rx;
	int hh = sty->i_fy;
	int dd = sty->i_uz;

	bg = getrandom()&0xffffff;
	fg = (~bg)&0xffffff;
	drawsolid_rect(win, bg, cx-ww, cy-hh, cx+ww, cy+hh);
	drawhexadecimal(win, fg, cx, cy, bg);
}
static void example_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
}
static void example_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
}
static void example_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
}
static void example_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)example_read_cli(win, sty, act, com);
	else if(fmt == _tui_)example_read_tui(win, sty, act, com);
	else if(fmt == _html_)example_read_html(win, sty, act, com);
	else if(fmt == _vbo_)example_read_vbo(win, sty, act, com);
	else example_read_pixel(win, sty, act, com);
}
static void example_write(
	struct actor* act, struct compo* com,
	struct event* ev)
{
}




static void example_list()
{
}
static void example_change()
{
}
static void example_stop()
{
}
static void example_start()
{
}
static void example_delete()
{
}
static void example_create()
{
}




void example_register(struct actor* p)
{
	p->type = hex32('t', 'e', 's', 't');
	p->name = hex64('e', 'x', 'a', 'm', 'p', 'l', 'e', 0);
	p->irel = 0;
	p->orel = 0;

	p->oncreate = (void*)example_create;
	p->ondelete = (void*)example_delete;
	p->onstart  = (void*)example_start;
	p->onstop   = (void*)example_stop;
	p->onlist   = (void*)example_list;
	p->onchoose = (void*)example_change;
	p->onread   = (void*)example_read;
	p->onwrite  = (void*)example_write;
}