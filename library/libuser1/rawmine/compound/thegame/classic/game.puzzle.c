#include "libuser.h"



static u8 buffer[16];




static void puzzle_read(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void puzzle_write(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void puzzle_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void puzzle_start(struct halfrel* self, struct halfrel* peer)
{
}




static void puzzle_search(struct actor* act)
{
}
static void puzzle_modify(struct actor* act)
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
	if(_copy_ == act->type)act->buf = memorycreate(16, 0);
}




void puzzle_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('p', 'u', 'z', 'z', 'l', 'e', 0, 0);

	p->oncreate = (void*)puzzle_create;
	p->ondelete = (void*)puzzle_delete;
	p->onsearch = (void*)puzzle_search;
	p->onmodify = (void*)puzzle_modify;

	p->onstart = (void*)puzzle_start;
	p->onstop  = (void*)puzzle_stop;
	p->onread  = (void*)puzzle_read;
	p->onwrite = (void*)puzzle_write;
}
