#include "libuser.h"



static u8 buffer[16];




static void gobang_sread(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void gobang_swrite(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void gobang_cread(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void gobang_cwrite(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void gobang_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void gobang_start(struct halfrel* self, struct halfrel* peer)
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
	p->oncread  = (void*)gobang_cread;
	p->oncwrite = (void*)gobang_cwrite;
	p->onsread  = (void*)gobang_sread;
	p->onswrite = (void*)gobang_swrite;
}
