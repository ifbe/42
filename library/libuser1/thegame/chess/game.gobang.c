#include "libuser.h"



static u8 buffer[16];




static void gobang_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void gobang_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void gobang_get()
{
}
static void gobang_post()
{
}
static void gobang_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
}
static void gobang_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
}
static void gobang_delete(struct actor* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memorydelete(act->buf);
}
static void gobang_create(struct actor* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->buf = buffer;
	if(_copy_ == act->type)act->buf = memorycreate(16);
}




void gobang_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('g', 'o', 'b', 'a', 'n', 'g', 0, 0);

	p->oncreate = (void*)gobang_create;
	p->ondelete = (void*)gobang_delete;
	p->onstart  = (void*)gobang_start;
	p->onstop   = (void*)gobang_stop;
	p->onget    = (void*)gobang_get;
	p->onpost   = (void*)gobang_post;
	p->onread   = (void*)gobang_read;
	p->onwrite  = (void*)gobang_write;
}
