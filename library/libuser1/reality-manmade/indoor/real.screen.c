#include "libuser.h"




static void screen_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void screen_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void screen_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void screen_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void screen_search(_obj* act)
{
}
static void screen_modify(_obj* act)
{
}
static void screen_delete(_obj* act)
{
}
static void screen_create(_obj* act, void* str)
{
}




void screen_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex64('s', 'c', 'r', 'e', 'e', 'n', 0, 0);

	p->oncreate = (void*)screen_create;
	p->ondelete = (void*)screen_delete;
	p->onreader = (void*)screen_search;
	p->onwriter = (void*)screen_modify;

	p->onattach = (void*)screen_attach;
	p->ondetach = (void*)screen_detach;
	p->ontaking = (void*)screen_taking;
	p->ongiving = (void*)screen_giving;
}
