#include "libuser.h"




static void stereo_taking(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void stereo_giving(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void stereo_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void stereo_linkup(struct halfrel* self, struct halfrel* peer)
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
	p->onsearch = (void*)stereo_search;
	p->onmodify = (void*)stereo_modify;

	p->onlinkup = (void*)stereo_linkup;
	p->ondiscon = (void*)stereo_discon;
	p->ontaking = (void*)stereo_taking;
	p->ongiving = (void*)stereo_giving;
}
