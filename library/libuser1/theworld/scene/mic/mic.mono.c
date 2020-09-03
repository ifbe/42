#include "libuser.h"




static void monomic_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void monomic_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void monomic_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void monomic_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void monomic_search(struct entity* act)
{
}
static void monomic_modify(struct entity* act)
{
}
static void monomic_delete(struct entity* act)
{
}
static void monomic_create(struct entity* act)
{
}




void monomic_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('m', 'o', 'n', 'o', 'm', 'i', 'c', 0);

	p->oncreate = (void*)monomic_create;
	p->ondelete = (void*)monomic_delete;
	p->onsearch = (void*)monomic_search;
	p->onmodify = (void*)monomic_modify;

	p->onlinkup = (void*)monomic_linkup;
	p->ondiscon = (void*)monomic_discon;
	p->ontaking = (void*)monomic_taking;
	p->ongiving = (void*)monomic_giving;
}
