#include "libuser.h"




static void stereo_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void stereo_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void stereo_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void stereo_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void stereo_search(_obj* act)
{
}
static void stereo_modify(_obj* act)
{
}
static void stereo_delete(_obj* act)
{
}
static void stereo_create(_obj* act)
{
}




void stereo_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('s', 't', 'e', 'r', 'e', 'o', 0, 0);

	p->oncreate = (void*)stereo_create;
	p->ondelete = (void*)stereo_delete;
	p->onreader = (void*)stereo_search;
	p->onwriter = (void*)stereo_modify;

	p->onattach = (void*)stereo_attach;
	p->ondetach = (void*)stereo_detach;
	p->ontaking = (void*)stereo_taking;
	p->ongiving = (void*)stereo_giving;
}
