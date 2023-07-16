#include "libuser.h"



static u8 buffer[16];




static void gobang_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void gobang_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void gobang_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void gobang_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void gobang_search(_obj* act)
{
}
static void gobang_modify(_obj* act)
{
}
static void gobang_delete(_obj* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memorydelete(act->listptr.buf0);
}
static void gobang_create(_obj* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->listptr.buf0 = buffer;
	if(_copy_ == act->type)act->listptr.buf0 = memorycreate(16, 0);
}




void gobang_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('g', 'o', 'b', 'a', 'n', 'g', 0, 0);

	p->oncreate = (void*)gobang_create;
	p->ondelete = (void*)gobang_delete;
	p->onreader = (void*)gobang_search;
	p->onwriter = (void*)gobang_modify;

	p->onattach = (void*)gobang_attach;
	p->ondetach = (void*)gobang_detach;
	p->ontaking = (void*)gobang_taking;
	p->ongiving = (void*)gobang_giving;
}
