#include "actor.h"



static u8 buffer[16];




static void brick_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void brick_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void brick_stop(struct actor* act, struct pinid* pin)
{
}
static void brick_start(struct actor* act, struct pinid* pin)
{
}
static void brick_delete(struct actor* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)stopmemory(act->buf);
}
static void brick_create(struct actor* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->buf = buffer;
	if(_copy_ == act->type)act->buf = startmemory(16);
}
static void brick_list()
{
}
static void brick_choose()
{
}




void brick_register(struct actor* p)
{
	p->type = _orig_;
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
