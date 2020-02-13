#include "libuser.h"




static void chippin_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void chippin_draw_gl41(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void chippin_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void chippin_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void chippin_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void chippin_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void chippin_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
}
static void chippin_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
}
static void chippin_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void chippin_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void chippin_search(struct entity* act, u8* buf)
{
}
static void chippin_modify(struct entity* act, u8* buf)
{
}
static void chippin_delete(struct entity* act, u8* buf)
{
}
static void chippin_create(struct entity* act, u8* buf)
{
}




void chippin_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex32('p','i','n',0);

	p->oncreate = (void*)chippin_create;
	p->ondelete = (void*)chippin_delete;
	p->onsearch = (void*)chippin_search;
	p->onmodify = (void*)chippin_modify;

	p->onlinkup = (void*)chippin_linkup;
	p->ondiscon = (void*)chippin_discon;
	p->onread  = (void*)chippin_read;
	p->onwrite = (void*)chippin_write;
}
