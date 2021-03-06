#include "libuser.h"



static u8 buffer[16];




static void kriegspiel_taking(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void kriegspiel_giving(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void kriegspiel_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void kriegspiel_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void kriegspiel_search(struct entity* act)
{
}
static void kriegspiel_modify(struct entity* act)
{
}
static void kriegspiel_delete(struct entity* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memorydelete(act->buf0);
}
static void kriegspiel_create(struct entity* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->buf0 = buffer;
	if(_copy_ == act->type)act->buf0 = memorycreate(16, 0);
}




void kriegspiel_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('k', 'r', 'i', 'e', 'g', 0, 0, 0);

	p->oncreate = (void*)kriegspiel_create;
	p->ondelete = (void*)kriegspiel_delete;
	p->onsearch = (void*)kriegspiel_search;
	p->onmodify = (void*)kriegspiel_modify;

	p->onlinkup = (void*)kriegspiel_linkup;
	p->ondiscon = (void*)kriegspiel_discon;
	p->ontaking = (void*)kriegspiel_taking;
	p->ongiving = (void*)kriegspiel_giving;
}
