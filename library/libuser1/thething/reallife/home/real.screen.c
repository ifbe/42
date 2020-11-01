#include "libuser.h"




static void screen_taking(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void screen_giving(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void screen_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void screen_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void screen_search(struct entity* act)
{
}
static void screen_modify(struct entity* act)
{
}
static void screen_delete(struct entity* act)
{
}
static void screen_create(struct entity* act, void* str)
{
}




void screen_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('s', 'c', 'r', 'e', 'e', 'n', 0, 0);

	p->oncreate = (void*)screen_create;
	p->ondelete = (void*)screen_delete;
	p->onsearch = (void*)screen_search;
	p->onmodify = (void*)screen_modify;

	p->onlinkup = (void*)screen_linkup;
	p->ondiscon = (void*)screen_discon;
	p->ontaking = (void*)screen_taking;
	p->ongiving = (void*)screen_giving;
}
