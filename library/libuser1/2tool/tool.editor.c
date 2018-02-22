#include "actor.h"




static void editor_read_html(struct arena* win, struct actor* act, struct style* sty)
{
}
static void editor_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
}
static void editor_read_vbo(struct arena* win, struct actor* act, struct style* sty)
{
}
static void editor_read_tui(struct arena* win, struct actor* act, struct style* sty)
{
}
static void editor_read_cli(struct arena* win, struct actor* act, struct style* sty)
{
	say("editor(%x,%x,%x)\n",win,act,sty);
}
static void editor_read(struct arena* win, struct actor* act, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)editor_read_cli(win, act, sty);
	else if(fmt == _tui_)editor_read_tui(win, act, sty);
	else if(fmt == _html_)editor_read_html(win, act, sty);
	else if(fmt == _vbo_)editor_read_vbo(win, act, sty);
	else editor_read_pixel(win, act, sty);
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
	p->type = hex32('h', 'a', 'c', 'k');
	p->name = hex64('e', 'd', 'i', 't', 'o', 'r', 0, 0);
	p->irel = 0;
	p->orel = 0;

	p->onstart = (void*)editor_start;
	p->onstop = (void*)editor_stop;
	p->onlist = (void*)editor_list;
	p->onchoose = (void*)editor_choose;
	p->onread = (void*)editor_read;
	p->onwrite = (void*)editor_write;
}
void editor_delete()
{
}
