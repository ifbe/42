#include "libuser.h"



static u8 buffer[16];




static void kriegspiel_read(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void kriegspiel_write(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void kriegspiel_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void kriegspiel_start(struct halfrel* self, struct halfrel* peer)
{
}




static void kriegspiel_search(struct actor* act)
{
}
static void kriegspiel_modify(struct actor* act)
{
}
static void kriegspiel_delete(struct actor* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memorydelete(act->buf);
}
static void kriegspiel_create(struct actor* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->buf = buffer;
	if(_copy_ == act->type)act->buf = memorycreate(16, 0);
}




void kriegspiel_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('k', 'r', 'i', 'e', 'g', 0, 0, 0);

	p->oncreate = (void*)kriegspiel_create;
	p->ondelete = (void*)kriegspiel_delete;
	p->onsearch = (void*)kriegspiel_search;
	p->onmodify = (void*)kriegspiel_modify;

	p->onstart = (void*)kriegspiel_start;
	p->onstop  = (void*)kriegspiel_stop;
	p->onread  = (void*)kriegspiel_read;
	p->onwrite = (void*)kriegspiel_write;
}
