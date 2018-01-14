#include "actor.h"




static void graph_read_vbo(struct arena* win, struct actor* act, struct style* sty)
{
}
static void graph_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
	int cx = (sty->cx) * (win->w) / 0x10000;
	int cy = (sty->cy) * (win->h) / 0x10000;
	int ww = (sty->wantw) * (win->w) / 0x20000;
	int hh = (sty->wanth) * (win->h) / 0x20000;
	drawsolid_rect(win, 0x00ff00, cx-ww, cy-hh, cx+ww, cy+hh);
}
static void graph_read_html(struct arena* win, struct actor* act, struct style* sty)
{
}
static void graph_read_tui(struct arena* win, struct actor* act, struct style* sty)
{
}
static void graph_read_cli(struct arena* win, struct actor* act, struct style* sty)
{
}
static void graph_read(struct arena* win, struct actor* act, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == __cli__)graph_read_cli(win, act, sty);
	else if(fmt == __tui__)graph_read_tui(win, act, sty);
	else if(fmt == __html__)graph_read_html(win, act, sty);
	else if(fmt == __vbo__)graph_read_vbo(win, act, sty);
	else graph_read_pixel(win, act, sty);
}
static void graph_write(struct event* ev)
{
}




static void graph_list()
{
}
static void graph_change()
{
}
static void graph_start()
{
}
static void graph_stop()
{
}
void graph_create(void* base,void* addr)
{
	struct actor* p = addr;
	p->type = hex32('t', 'e', 's', 't');
	p->name = hex64('g', 'r', 'a', 'p', 'h', 0, 0, 0);

	p->start = (void*)graph_start;
	p->stop = (void*)graph_stop;
	p->list = (void*)graph_list;
	p->choose = (void*)graph_change;
	p->read = (void*)graph_read;
	p->write = (void*)graph_write;
}
void graph_delete()
{
}
