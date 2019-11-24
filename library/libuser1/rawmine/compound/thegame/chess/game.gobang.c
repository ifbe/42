#include "libuser.h"



static u8 buffer[16];




static void gobang_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void gobang_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void gobang_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void gobang_start(struct halfrel* self, struct halfrel* peer)
{
}




static void gobang_search(struct entity* act)
{
}
static void gobang_modify(struct entity* act)
{
}
static void gobang_delete(struct entity* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memorydelete(act->buf);
}
static void gobang_create(struct entity* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->buf = buffer;
	if(_copy_ == act->type)act->buf = memorycreate(16, 0);
}




void gobang_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('g', 'o', 'b', 'a', 'n', 'g', 0, 0);

	p->oncreate = (void*)gobang_create;
	p->ondelete = (void*)gobang_delete;
	p->onsearch = (void*)gobang_search;
	p->onmodify = (void*)gobang_modify;

	p->onstart = (void*)gobang_start;
	p->onstop  = (void*)gobang_stop;
	p->onread  = (void*)gobang_read;
	p->onwrite = (void*)gobang_write;
}
