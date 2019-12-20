#include "libuser.h"




static void monomic_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	struct entity* ent = self->pchip;
	struct supply* sup = peer->pchip;
	say("@monomic_read\n", ent, sup);
}
static void monomic_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void monomic_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void monomic_start(struct halfrel* self, struct halfrel* peer)
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

	p->onstart = (void*)monomic_start;
	p->onstop  = (void*)monomic_stop;
	p->onread  = (void*)monomic_read;
	p->onwrite = (void*)monomic_write;
}
