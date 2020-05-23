#include "libuser.h"




static void x8664_search(struct entity* act, u8* buf)
{
}
static void x8664_modify(struct entity* act, u8* buf)
{
}
static void x8664_delete(struct entity* act, u8* buf)
{
	//write cpustat to file

	//free 64m
}
static void x8664_create(struct entity* act, void* arg, int argc, u8** argv)
{
	//alloc 64m
	act->buf0 = memorycreate(0x100000, 0);
	if(0 == act->buf0)return;

	//read cpustat from file
}




static void x8664_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void x8664_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void x8664_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void x8664_linkup(struct halfrel* self, struct halfrel* peer)
{
}




void x8664_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('x','8','6','6','4', 0, 0, 0);

	p->oncreate = (void*)x8664_create;
	p->ondelete = (void*)x8664_delete;
	p->onsearch = (void*)x8664_search;
	p->onmodify = (void*)x8664_modify;

	p->onlinkup = (void*)x8664_linkup;
	p->ondiscon = (void*)x8664_discon;
	p->ontaking = (void*)x8664_taking;
	p->ongiving = (void*)x8664_giving;
}
