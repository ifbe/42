#include "libuser.h"




static void theeye_taking(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	say("@theeye_read\n");
}
static void theeye_giving(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void theeye_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void theeye_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void theeye_search(struct entity* act)
{
}
static void theeye_modify(struct entity* act)
{
}
static void theeye_delete(struct entity* act)
{
}
static void theeye_create(struct entity* act, void* str)
{
}




void theeye_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('t', 'h', 'e', 'e', 'y', 'e', 0, 0);

	p->oncreate = (void*)theeye_create;
	p->ondelete = (void*)theeye_delete;
	p->onsearch = (void*)theeye_search;
	p->onmodify = (void*)theeye_modify;

	p->onlinkup = (void*)theeye_linkup;
	p->ondiscon = (void*)theeye_discon;
	p->ontaking = (void*)theeye_taking;
	p->ongiving = (void*)theeye_giving;
}
