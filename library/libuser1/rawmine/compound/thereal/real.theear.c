#include "libuser.h"




static void theear_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	say("@theear_read\n");
}
static void theear_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void theear_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void theear_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void theear_search(struct entity* act)
{
}
static void theear_modify(struct entity* act)
{
}
static void theear_delete(struct entity* act)
{
}
static void theear_create(struct entity* act, void* str)
{
}




void theear_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('t', 'h', 'e', 'e', 'a', 'r', 0, 0);

	p->oncreate = (void*)theear_create;
	p->ondelete = (void*)theear_delete;
	p->onsearch = (void*)theear_search;
	p->onmodify = (void*)theear_modify;

	p->onlinkup = (void*)theear_linkup;
	p->ondiscon = (void*)theear_discon;
	p->onread  = (void*)theear_read;
	p->onwrite = (void*)theear_write;
}
