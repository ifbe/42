#include "libuser.h"




static void theeye_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	say("@theeye_read\n");
}
static void theeye_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void theeye_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void theeye_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void theeye_search(_obj* act)
{
}
static void theeye_modify(_obj* act)
{
}
static void theeye_delete(_obj* act)
{
}
static void theeye_create(_obj* act, void* str)
{
}




void theeye_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('t', 'h', 'e', 'e', 'y', 'e', 0, 0);

	p->oncreate = (void*)theeye_create;
	p->ondelete = (void*)theeye_delete;
	p->onreader = (void*)theeye_search;
	p->onwriter = (void*)theeye_modify;

	p->onattach = (void*)theeye_attach;
	p->ondetach = (void*)theeye_detach;
	p->ontaking = (void*)theeye_taking;
	p->ongiving = (void*)theeye_giving;
}
