#include "actor.h"




static void mahjong_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct compo* com)
{
}
static void mahjong_write(
	struct actor* act, struct compo* com,
	struct event* ev)
{
}
static void mahjong_list()
{
}
static void mahjong_choose()
{
}
static void mahjong_stop()
{
}
static void mahjong_start()
{
}
static void mahjong_delete()
{
}
static void mahjong_create()
{
}




void mahjong_register(struct actor* p)
{
	p->type = hex32('g', 'a', 'm', 'e');
	p->name = hex64('m', 'a', 'h', 'j', 'o', 'n', 'g', 0);
	p->irel = 0;
	p->orel = 0;

	p->oncreate = (void*)mahjong_create;
	p->ondelete = (void*)mahjong_delete;
	p->onstart  = (void*)mahjong_start;
	p->onstop   = (void*)mahjong_stop;
	p->onlist   = (void*)mahjong_list;
	p->onchoose = (void*)mahjong_choose;
	p->onread   = (void*)mahjong_read;
	p->onwrite  = (void*)mahjong_write;
}