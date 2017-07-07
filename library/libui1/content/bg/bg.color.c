#include "actor.h"
void backgroundcolor(void*, u32);




static void bgcolor_read_cli()
{
}
static void bgcolor_read_tui(struct arena* win, struct actor* act, struct relation* rel)
{
}
static void bgcolor_read_html(struct arena* win, struct actor* act, struct relation* rel)
{
}
static void bgcolor_read_pixel(struct arena* win, struct actor* act, struct relation* rel)
{
	backgroundcolor(win, 0);
}
static void bgcolor_read(struct relation* rel)
{
	struct arena* win = rel->parent_this;
	struct actor* act = rel->child_this;
	u64 fmt = win->fmt;
	u64 dim = win->dim;

	//cli
	if(dim == 1)bgcolor_read_cli();

	//text
	else if(fmt == 0x74786574)bgcolor_read_tui(win, act, rel);

	//html
	else if(fmt == 0x6c6d7468)bgcolor_read_html(win, act, rel);

	//pixel
	else bgcolor_read_pixel(win, act, rel);
}
static void bgcolor_write(struct event* ev)
{
	say("@bgcolor:%x,%x,%x,%x\n",ev->why,ev->what,ev->where,ev->when);
}
static void bgcolor_list()
{
}
static void bgcolor_choose()
{
}
static void bgcolor_stop()
{
}
static void bgcolor_start()
{
}
void bgcolor_delete()
{
}
void bgcolor_create(void* base, struct actor* act)
{
	act->type = hexof('b','g',0,0,0,0,0,0);
	act->name = hexof('b','g','c','o','l','o','r',0);
	act->first = 0;
	act->last = 0;

	act->start = (void*)bgcolor_start;
	act->stop = (void*)bgcolor_stop;
	act->list = (void*)bgcolor_list;
	act->choose = (void*)bgcolor_choose;
	act->read = (void*)bgcolor_read;
	act->write = (void*)bgcolor_write;
}