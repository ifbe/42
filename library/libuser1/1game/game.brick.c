#include "actor.h"




static void brick_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
}
static void brick_write(
	struct actor* act, struct compo* com,
	struct event* ev)
{
}
static void brick_list()
{
}
static void brick_choose()
{
}
static void brick_stop()
{
}
static void brick_start()
{
}
static void brick_delete()
{
}
static void brick_create()
{
}




void brick_register(struct actor* p)
{
	p->type = hex32('g', 'a', 'm', 'e');
	p->name = hex64('b', 'r', 'i', 'c', 'k', 0, 0, 0);
	p->irel = 0;
	p->orel = 0;

	p->oncreate = (void*)brick_create;
	p->ondelete = (void*)brick_delete;
	p->onstart  = (void*)brick_start;
	p->onstop   = (void*)brick_stop;
	p->onlist   = (void*)brick_list;
	p->onchoose = (void*)brick_choose;
	p->onread   = (void*)brick_read;
	p->onwrite  = (void*)brick_write;
}