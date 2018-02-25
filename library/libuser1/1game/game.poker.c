#include "actor.h"




static void poker_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
}
static void poker_write(
	struct actor* act, struct compo* com,
	struct event* ev)
{
}
static void poker_list()
{
}
static void poker_choose()
{
}
static void poker_stop()
{
}
static void poker_start()
{
}
static void poker_delete()
{
}
static void poker_create()
{
}




void poker_register(struct actor* p)
{
	p->type = hex32('g', 'a', 'm', 'e');
	p->name = hex64('p', 'o', 'k', 'e', 'r', 0, 0, 0);
	p->irel = 0;
	p->orel = 0;

	p->oncreate = (void*)poker_create;
	p->ondelete = (void*)poker_delete;
	p->onstart  = (void*)poker_start;
	p->onstop   = (void*)poker_stop;
	p->onlist   = (void*)poker_list;
	p->onchoose = (void*)poker_choose;
	p->onread   = (void*)poker_read;
	p->onwrite  = (void*)poker_write;
}