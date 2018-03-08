#include "actor.h"
u32 getrandom();




static void example_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int cx = sty->cx;
	int cy = sty->cy;
	int cz = sty->cz;
	int ww = sty->rx;
	int hh = sty->fy;
	int dd = sty->uz;
	carvesolid_rect(
		win, getrandom(),
		cx, cy, cz,
		ww, 0.0, 0.0,
		0.0, hh, 0.0
	);
}
static void example_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u32 bg,fg;
	int cx = sty->cx;
	int cy = sty->cy;
	int cz = sty->cz;
	int ww = sty->rx;
	int hh = sty->fy;
	int dd = sty->uz;

	bg = getrandom()&0xffffff;
	fg = (~bg)&0xffffff;
	drawsolid_rect(win, bg, cx-ww, cy-hh, cx+ww, cy+hh);
	drawhexadecimal(win, fg, cx, cy, bg);
}
static void example_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void example_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void example_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}




static void example_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)example_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)example_read_tui(win, sty, act, pin);
	else if(fmt == _html_)example_read_html(win, sty, act, pin);
	else if(fmt == _vbo_)example_read_vbo(win, sty, act, pin);
	else example_read_pixel(win, sty, act, pin);
}
static void example_write(
	struct actor* act, struct pinid* pin,
	struct event* ev)
{
}
static void example_list()
{
}
static void example_change()
{
}
static void example_stop(struct actor* act, struct pinid* pin)
{
}
static void example_start(struct actor* act, struct pinid* pin)
{
}
static void example_delete(struct actor* act)
{
	if(0 == act)return;
	//if(_orig_ == act->type)act->buf = buffer;
	//if(_copy_ == act->type)act->buf = startmemory(256);
}
static void example_create(struct actor* act)
{
	if(0 == act)return;
	//if(_orig_ == act->type)act->buf = buffer;
	//if(_copy_ == act->type)act->buf = startmemory(256);
}




void example_register(struct actor* p)
{
	p->type = _orig_;
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
