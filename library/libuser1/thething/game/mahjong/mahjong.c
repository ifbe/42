#include "libuser.h"



static u8 data[144];




static void mahjong_taking(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void mahjong_giving(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void mahjong_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void mahjong_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void mahjong_search(struct entity* act)
{
}
static void mahjong_modify(struct entity* act)
{
}
static void mahjong_delete(struct entity* act)
{
	if(0 == act)return;
	if(act->buf0){
		memorydelete(act->buf0);
		act->buf0 = 0;
	}
}
static void mahjong_create(struct entity* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->buf0 = data;
	if(_copy_ == act->type)act->buf0 = memorycreate(144, 0);
}




void mahjong_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('m', 'a', 'h', 'j', 'o', 'n', 'g', 0);

	p->oncreate = (void*)mahjong_create;
	p->ondelete = (void*)mahjong_delete;
	p->onsearch = (void*)mahjong_search;
	p->onmodify = (void*)mahjong_modify;

	p->onlinkup = (void*)mahjong_linkup;
	p->ondiscon = (void*)mahjong_discon;
	p->ontaking = (void*)mahjong_taking;
	p->ongiving = (void*)mahjong_giving;
}
