#include "actor.h"




static void starry_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
	int cx = (win->w) * (sty->cx) / 0x10000;
	int cy = (win->h) * (sty->cy) / 0x10000;
	int ww = (win->w) * (sty->wantw) / 0x20000;
	int hh = (win->h) * (sty->wanth) / 0x20000;
	drawsolid_rect(win, 0x222222, cx-ww, cy-hh, cx+ww, cy+hh);
	drawicon_1(win, 0x00ff00, cx-32, cy-16, cx+32, cy+16, "123", 0);
}
static void starry_read_html(struct arena* win, struct actor* act, struct style* sty)
{
}
static void starry_read_vbo(struct arena* win, struct actor* act, struct style* sty)
{
}
static void starry_read_tui(struct arena* win, struct actor* act, struct style* sty)
{
}
static void starry_read_cli(struct arena* win, struct actor* act, struct style* sty)
{
	say("starry(%x,%x,%x)\n",win,act,sty);
}
static void starry_read(struct arena* win, struct actor* act, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == __cli__)starry_read_cli(win, act, sty);
	else if(fmt == __tui__)starry_read_tui(win, act, sty);
	else if(fmt == __html__)starry_read_html(win, act, sty);
	else if(fmt == __vbo__)starry_read_vbo(win, act, sty);
	else starry_read_pixel(win, act, sty);
}
static void starry_write(struct event* ev)
{
}




static void starry_list()
{
}
static void starry_into()
{
}
static void starry_start()
{
}
static void starry_stop()
{
}
void starry_create(void* base,void* addr)
{
	struct actor* act = addr;
	//node = (struct starry*)(base+0x300000);

	act->type = hex32('h', 'a', 'c', 'k');
	act->name = hex64('s', 't', 'a', 'r', 'r', 'y', 0, 0);

	act->start = (void*)starry_start;
	act->stop = (void*)starry_stop;
	act->list = (void*)starry_list;
	act->choose = (void*)starry_into;
	act->read = (void*)starry_read;
	act->write = (void*)starry_write;
}
void starry_delete()
{
}
