#include "actor.h"
u32 getrandom();




static void example_read_vbo(struct arena* win, struct actor* act, struct style* sty)
{
}
static void example_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
	u32 bg,fg;
	int cx = (sty->cx) * (win->w) / 0x10000;
	int cy = (sty->cy) * (win->h) / 0x10000;
	int ww = (sty->wantw) * (win->w) / 0x20000;
	int hh = (sty->wanth) * (win->h) / 0x20000;

	bg = getrandom()&0xffffff;
	fg = (~bg)&0xffffff;
	drawsolid_rect(win, bg, cx-ww, cy-hh, cx+ww, cy+hh);
	drawhexadecimal(win, fg, cx, cy, bg);
}
static void example_read_html(struct arena* win, struct actor* act, struct style* sty)
{
}
static void example_read_tui(struct arena* win, struct actor* act, struct style* sty)
{
}
static void example_read_cli(struct arena* win, struct actor* act, struct style* sty)
{
}
static void example_read(struct arena* win, struct actor* act, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)example_read_cli(win, act, sty);
	else if(fmt == _tui_)example_read_tui(win, act, sty);
	else if(fmt == _html_)example_read_html(win, act, sty);
	else if(fmt == _vbo_)example_read_vbo(win, act, sty);
	else example_read_pixel(win, act, sty);
}
static void example_write(struct event* ev)
{
}




static void example_list()
{
}
static void example_change()
{
}
static void example_start()
{
}
static void example_stop()
{
}
void example_create(void* base,void* addr)
{
	struct actor* p = addr;
	p->type = hex32('t', 'e', 's', 't');
	p->name = hex64('e', 'x', 'a', 'm', 'p', 'l', 'e', 0);

	p->start = (void*)example_start;
	p->stop = (void*)example_stop;
	p->list = (void*)example_list;
	p->choose = (void*)example_change;
	p->read = (void*)example_read;
	p->write = (void*)example_write;
}
void example_delete()
{
}
