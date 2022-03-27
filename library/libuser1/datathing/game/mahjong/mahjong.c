#include "libuser.h"



static u8 data[144];




static void mahjong_taking(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void mahjong_giving(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void mahjong_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void mahjong_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void mahjong_search(_obj* act)
{
}
static void mahjong_modify(_obj* act)
{
}
static void mahjong_delete(_obj* act)
{
	if(0 == act)return;
	if(act->listptr.buf0){
		memorydelete(act->listptr.buf0);
		act->listptr.buf0 = 0;
	}
}
static void mahjong_create(_obj* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->listptr.buf0 = data;
	if(_copy_ == act->type)act->listptr.buf0 = memorycreate(144, 0);
}




void mahjong_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('m', 'a', 'h', 'j', 'o', 'n', 'g', 0);

	p->oncreate = (void*)mahjong_create;
	p->ondelete = (void*)mahjong_delete;
	p->onreader = (void*)mahjong_search;
	p->onwriter = (void*)mahjong_modify;

	p->onattach = (void*)mahjong_attach;
	p->ondetach = (void*)mahjong_detach;
	p->ontaking = (void*)mahjong_taking;
	p->ongiving = (void*)mahjong_giving;
}
