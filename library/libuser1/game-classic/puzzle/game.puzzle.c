#include "libuser.h"



static u8 buffer[16];




static void puzzle_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void puzzle_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void puzzle_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void puzzle_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void puzzle_search(_obj* act)
{
}
static void puzzle_modify(_obj* act)
{
}
static void puzzle_delete(_obj* act)
{
	if(0 == act)return;
	if(act->listptr.buf0){
		memoryfree(act->listptr.buf0);
		act->listptr.buf0 = 0;
	}
}
static void puzzle_create(_obj* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->listptr.buf0 = buffer;
	if(_copy_ == act->type)act->listptr.buf0 = memoryalloc(16, 0);
}




void puzzle_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('p', 'u', 'z', 'z', 'l', 'e', 0, 0);

	p->oncreate = (void*)puzzle_create;
	p->ondelete = (void*)puzzle_delete;
	p->onreader = (void*)puzzle_search;
	p->onwriter = (void*)puzzle_modify;

	p->onattach = (void*)puzzle_attach;
	p->ondetach = (void*)puzzle_detach;
	p->ontaking = (void*)puzzle_taking;
	p->ongiving = (void*)puzzle_giving;
}
