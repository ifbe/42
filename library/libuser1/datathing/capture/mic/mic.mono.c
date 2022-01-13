#include "libuser.h"




static void monomic_taking(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void monomic_giving(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void monomic_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void monomic_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void monomic_search(_obj* act)
{
}
static void monomic_modify(_obj* act)
{
}
static void monomic_delete(_obj* act)
{
}
static void monomic_create(_obj* act)
{
}




void monomic_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('m', 'o', 'n', 'o', 'm', 'i', 'c', 0);

	p->oncreate = (void*)monomic_create;
	p->ondelete = (void*)monomic_delete;
	p->onsearch = (void*)monomic_search;
	p->onmodify = (void*)monomic_modify;

	p->onattach = (void*)monomic_attach;
	p->ondetach = (void*)monomic_detach;
	p->ontaking = (void*)monomic_taking;
	p->ongiving = (void*)monomic_giving;
}
