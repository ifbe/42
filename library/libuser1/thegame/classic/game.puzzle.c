#include "libuser.h"



static u8 buffer[16];




static void puzzle_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void puzzle_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void puzzle_get()
{
}
static void puzzle_post()
{
}
static void puzzle_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
}
static void puzzle_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
}
static void puzzle_delete(struct actor* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memorydelete(act->buf);
}
static void puzzle_create(struct actor* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->buf = buffer;
	if(_copy_ == act->type)act->buf = memorycreate(16);
}




void puzzle_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('p', 'u', 'z', 'z', 'l', 'e', 0, 0);

	p->oncreate = (void*)puzzle_create;
	p->ondelete = (void*)puzzle_delete;
	p->onstart  = (void*)puzzle_start;
	p->onstop   = (void*)puzzle_stop;
	p->onget    = (void*)puzzle_get;
	p->onpost   = (void*)puzzle_post;
	p->onread   = (void*)puzzle_read;
	p->onwrite  = (void*)puzzle_write;
}
