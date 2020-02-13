#include "libuser.h"




static void screen_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void screen_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
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
	p->onread  = (void*)screen_read;
	p->onwrite = (void*)screen_write;
}
