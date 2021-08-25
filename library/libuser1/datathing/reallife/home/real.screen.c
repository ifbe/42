#include "libuser.h"




static void screen_taking(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void screen_giving(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void screen_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void screen_linkup(struct halfrel* self, struct halfrel* peer)
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
	p->type = _orig_;
	p->hfmt = hex64('s', 'c', 'r', 'e', 'e', 'n', 0, 0);

	p->oncreate = (void*)screen_create;
	p->ondelete = (void*)screen_delete;
	p->onsearch = (void*)screen_search;
	p->onmodify = (void*)screen_modify;

	p->onlinkup = (void*)screen_linkup;
	p->ondiscon = (void*)screen_discon;
	p->ontaking = (void*)screen_taking;
	p->ongiving = (void*)screen_giving;
}
