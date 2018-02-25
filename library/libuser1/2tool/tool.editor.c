#include "actor.h"




static void editor_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
}
static void editor_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
}
static void editor_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
}
static void editor_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
}
static void editor_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
	say("editor(%x,%x,%x)\n",win,act,sty);
}
static void editor_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)editor_read_cli(win, sty, act, com);
	else if(fmt == _tui_)editor_read_tui(win, sty, act, com);
	else if(fmt == _html_)editor_read_html(win, sty, act, com);
	else if(fmt == _vbo_)editor_read_vbo(win, sty, act, com);
	else editor_read_pixel(win, sty, act, com);
}
static void editor_write(
	struct actor* act, struct compo* com,
	struct event* ev)
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
static void editor_delete()
{
}
static void editor_create()
{
}




void editor_register(struct actor* p)
{
	p->type = hex32('h', 'a', 'c', 'k');
	p->name = hex64('e', 'd', 'i', 't', 'o', 'r', 0, 0);
	p->irel = 0;
	p->orel = 0;

	p->oncreate = (void*)editor_create;
	p->ondelete = (void*)editor_delete;
	p->onstart  = (void*)editor_start;
	p->onstop   = (void*)editor_stop;
	p->onlist   = (void*)editor_list;
	p->onchoose = (void*)editor_choose;
	p->onread   = (void*)editor_read;
	p->onwrite  = (void*)editor_write;
}