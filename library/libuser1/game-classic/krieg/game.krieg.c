#include "libuser.h"



static u8 buffer[16];




static void kriegspiel_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void kriegspiel_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void kriegspiel_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void kriegspiel_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void kriegspiel_search(_obj* act)
{
}
static void kriegspiel_modify(_obj* act)
{
}
static void kriegspiel_delete(_obj* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memoryfree(act->listptr.buf0);
}
static void kriegspiel_create(_obj* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->listptr.buf0 = buffer;
	if(_copy_ == act->type)act->listptr.buf0 = memoryalloc(16, 0);
}




void kriegspiel_register(_obj* p)
{
	p->kind = _game_;
	p->type = hex64('k', 'r', 'i', 'e', 'g', 0, 0, 0);
	p->vfmt = _orig_;

	p->oncreate = (void*)kriegspiel_create;
	p->ondelete = (void*)kriegspiel_delete;
	p->onreader = (void*)kriegspiel_search;
	p->onwriter = (void*)kriegspiel_modify;

	p->onattach = (void*)kriegspiel_attach;
	p->ondetach = (void*)kriegspiel_detach;
	p->ontaking = (void*)kriegspiel_taking;
	p->ongiving = (void*)kriegspiel_giving;
}
