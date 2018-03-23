#include "actor.h"



static u8 data[144];




static void mahjong_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void mahjong_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void mahjong_list()
{
}
static void mahjong_choose()
{
}
static void mahjong_stop(struct actor* act, struct pinid* pin)
{
}
static void mahjong_start(struct actor* act, struct pinid* pin)
{
}
static void mahjong_delete(struct actor* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)stopmemory(act->buf);
}
static void mahjong_create(struct actor* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->buf = data;
	if(_copy_ == act->type)act->buf = startmemory(144);
}




void mahjong_register(struct actor* p)
{
	p->type = _orig_;
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
