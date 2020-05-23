#include "libuser.h"



static u8 buffer[16];




static void gobang_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void gobang_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void gobang_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void gobang_linkup(struct halfrel* self, struct halfrel* peer)
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
	if(_copy_ == act->type)memorydelete(act->buf0);
}
static void gobang_create(struct entity* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->buf0 = buffer;
	if(_copy_ == act->type)act->buf0 = memorycreate(16, 0);
}




void gobang_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('g', 'o', 'b', 'a', 'n', 'g', 0, 0);

	p->oncreate = (void*)gobang_create;
	p->ondelete = (void*)gobang_delete;
	p->onsearch = (void*)gobang_search;
	p->onmodify = (void*)gobang_modify;

	p->onlinkup = (void*)gobang_linkup;
	p->ondiscon = (void*)gobang_discon;
	p->ontaking = (void*)gobang_taking;
	p->ongiving = (void*)gobang_giving;
}
