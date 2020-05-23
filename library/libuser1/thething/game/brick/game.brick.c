#include "libuser.h"



static u8 buffer[16];




static void brick_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void brick_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void brick_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void brick_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void brick_search(struct entity* act)
{
}
static void brick_modify(struct entity* act)
{
}
static void brick_delete(struct entity* act)
{
	if(0 == act)return;
	if(_copy_ == act->type){
		memorydelete(act->buf0);
		act->buf0 = 0;
	}
}
static void brick_create(struct entity* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->buf0 = buffer;
	if(_copy_ == act->type)act->buf0 = memorycreate(16, 0);
}




void brick_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('b', 'r', 'i', 'c', 'k', 0, 0, 0);

	p->oncreate = (void*)brick_create;
	p->ondelete = (void*)brick_delete;
	p->onsearch = (void*)brick_search;
	p->onmodify = (void*)brick_modify;

	p->onlinkup = (void*)brick_linkup;
	p->ondiscon = (void*)brick_discon;
	p->ontaking = (void*)brick_taking;
	p->ongiving = (void*)brick_giving;
}
