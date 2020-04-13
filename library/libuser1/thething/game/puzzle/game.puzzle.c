#include "libuser.h"



static u8 buffer[16];




static void puzzle_read(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void puzzle_write(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void puzzle_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void puzzle_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void puzzle_search(struct entity* act)
{
}
static void puzzle_modify(struct entity* act)
{
}
static void puzzle_delete(struct entity* act)
{
	if(0 == act)return;
	if(act->buf0){
		memorydelete(act->buf0);
		act->buf0 = 0;
	}
}
static void puzzle_create(struct entity* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->buf0 = buffer;
	if(_copy_ == act->type)act->buf0 = memorycreate(16, 0);
}




void puzzle_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('p', 'u', 'z', 'z', 'l', 'e', 0, 0);

	p->oncreate = (void*)puzzle_create;
	p->ondelete = (void*)puzzle_delete;
	p->onsearch = (void*)puzzle_search;
	p->onmodify = (void*)puzzle_modify;

	p->onlinkup = (void*)puzzle_linkup;
	p->ondiscon = (void*)puzzle_discon;
	p->onread  = (void*)puzzle_read;
	p->onwrite = (void*)puzzle_write;
}
