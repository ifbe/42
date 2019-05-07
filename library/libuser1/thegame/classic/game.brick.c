#include "libuser.h"



static u8 buffer[16];




static void brick_sread(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void brick_swrite(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void brick_cread(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void brick_cwrite(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void brick_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void brick_start(struct halfrel* self, struct halfrel* peer)
{
}
static void brick_delete(struct actor* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memorydelete(act->buf);
}
static void brick_create(struct actor* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->buf = buffer;
	if(_copy_ == act->type)act->buf = memorycreate(16);
}




void brick_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('b', 'r', 'i', 'c', 'k', 0, 0, 0);

	p->oncreate = (void*)brick_create;
	p->ondelete = (void*)brick_delete;
	p->onstart  = (void*)brick_start;
	p->onstop   = (void*)brick_stop;
	p->oncread  = (void*)brick_cread;
	p->oncwrite = (void*)brick_cwrite;
	p->onsread  = (void*)brick_sread;
	p->onswrite = (void*)brick_swrite;
}
