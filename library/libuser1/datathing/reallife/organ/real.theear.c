#include "libuser.h"




static void theear_taking(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	say("@theear_read\n");
}
static void theear_giving(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void theear_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void theear_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void theear_search(_obj* act)
{
}
static void theear_modify(_obj* act)
{
}
static void theear_delete(_obj* act)
{
}
static void theear_create(_obj* act, void* str)
{
}




void theear_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('t', 'h', 'e', 'e', 'a', 'r', 0, 0);

	p->oncreate = (void*)theear_create;
	p->ondelete = (void*)theear_delete;
	p->onsearch = (void*)theear_search;
	p->onmodify = (void*)theear_modify;

	p->onlinkup = (void*)theear_linkup;
	p->ondiscon = (void*)theear_discon;
	p->ontaking = (void*)theear_taking;
	p->ongiving = (void*)theear_giving;
}
