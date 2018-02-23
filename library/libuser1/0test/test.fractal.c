#include "actor.h"




static void fractal_read_html(struct arena* win, struct actor* act, struct style* sty)
{
}
static void fractal_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
	int cx = sty->i_cx;
	int cy = sty->i_cy;
	int cz = sty->i_cz;
	int ww = sty->i_rx;
	int hh = sty->i_fy;
	int dd = sty->i_uz;

	drawsolid_rect(win, 0xff0000, cx-ww, cy-hh, cx+ww, cy+hh);
	drawline_rect( win, 0x00ff00, cx-ww, cy-hh, cx+ww, cy+hh);
}
static void fractal_read_vbo(struct arena* win, struct actor* act, struct style* sty)
{
}
static void fractal_read_tui(struct arena* win, struct actor* act, struct style* sty)
{
}
static void fractal_read_cli(struct arena* win, struct actor* act, struct style* sty)
{
	say("fractal(%x,%x,%x)\n",win,act,sty);
}
static void fractal_read(struct arena* win, struct actor* act, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)fractal_read_cli(win, act, sty);
	else if(fmt == _tui_)fractal_read_tui(win, act, sty);
	else if(fmt == _html_)fractal_read_html(win, act, sty);
	else if(fmt == _vbo_)fractal_read_vbo(win, act, sty);
	else fractal_read_pixel(win, act, sty);
}
static void fractal_write(struct event* ev)
{
}




static void fractal_list()
{
}
static void fractal_change()
{
}
static void fractal_stop()
{
}
static void fractal_start()
{
}
static void fractal_delete()
{
}
static void fractal_create()
{
}




void fractal_register(struct actor* p)
{
	p->type = hex32('t', 'e', 's', 't');
	p->name = hex64('f', 'r', 'a', 'c', 't', 'a', 'l', 0);
	p->irel = 0;
	p->orel = 0;

	p->oncreate = (void*)fractal_create;
	p->ondelete = (void*)fractal_delete;
	p->onstart  = (void*)fractal_start;
	p->onstop   = (void*)fractal_stop;
	p->onlist   = (void*)fractal_list;
	p->onchoose = (void*)fractal_change;
	p->onread   = (void*)fractal_read;
	p->onwrite  = (void*)fractal_write;
}