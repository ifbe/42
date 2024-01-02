#include "libuser.h"




static void x8664_search(_obj* act, u8* buf)
{
}
static void x8664_modify(_obj* act, u8* buf)
{
}
static void x8664_delete(_obj* act, u8* buf)
{
	//write cpustat to file

	//free 64m
}
static void x8664_create(_obj* act, void* arg, int argc, u8** argv)
{
	//alloc 64m
	act->listptr.buf0 = memoryalloc(0x100000, 0);
	if(0 == act->listptr.buf0)return;

	//read cpustat from file
}




static void x8664_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void x8664_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void x8664_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void x8664_attach(struct halfrel* self, struct halfrel* peer)
{
}




void x8664_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('x','8','6','6','4', 0, 0, 0);

	p->oncreate = (void*)x8664_create;
	p->ondelete = (void*)x8664_delete;
	p->onreader = (void*)x8664_search;
	p->onwriter = (void*)x8664_modify;

	p->onattach = (void*)x8664_attach;
	p->ondetach = (void*)x8664_detach;
	p->ontaking = (void*)x8664_taking;
	p->ongiving = (void*)x8664_giving;
}
