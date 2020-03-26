#include "libuser.h"




static void stereo_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	struct entity* ent = self->pchip;
	struct supply* sup = peer->pchip;
	say("@stereo_read\n", ent, sup);
}
static void stereo_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void stereo_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void stereo_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void stereo_search(struct entity* act)
{
}
static void stereo_modify(struct entity* act)
{
}
static void stereo_delete(struct entity* act)
{
}
static void stereo_create(struct entity* act)
{
}




void stereo_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('s', 't', 'e', 'r', 'e', 'o', 0, 0);

	p->oncreate = (void*)stereo_create;
	p->ondelete = (void*)stereo_delete;
	p->onsearch = (void*)stereo_search;
	p->onmodify = (void*)stereo_modify;

	p->onlinkup = (void*)stereo_linkup;
	p->ondiscon = (void*)stereo_discon;
	p->onread  = (void*)stereo_read;
	p->onwrite = (void*)stereo_write;
}
