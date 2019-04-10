#include "libuser.h"



static u8 buffer[16];




static void puzzle_sread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
}
static void puzzle_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void puzzle_cread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void puzzle_cwrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
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
	p->fmt = hex64('p', 'u', 'z', 'z', 'l', 'e', 0, 0);

	p->oncreate = (void*)puzzle_create;
	p->ondelete = (void*)puzzle_delete;
	p->onstart  = (void*)puzzle_start;
	p->onstop   = (void*)puzzle_stop;
	p->oncread  = (void*)puzzle_cread;
	p->oncwrite = (void*)puzzle_cwrite;
	p->onsread  = (void*)puzzle_sread;
	p->onswrite = (void*)puzzle_swrite;
}
