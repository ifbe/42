#include "libuser.h"




static void wire_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void wire_draw_gl41(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void wire_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void wire_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void wire_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void wire_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void wire_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	//if 'draw' == self.foot
	struct entity* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct entity* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	//wire_draw(act, pin, win, sty);
}
static void wire_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
}
static void wire_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void wire_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void wire_search(struct entity* act, u8* buf)
{
}
static void wire_modify(struct entity* act, u8* buf)
{
}
static void wire_delete(struct entity* act, u8* buf)
{
}
static void wire_create(struct entity* act, u8* buf)
{
}




void wire_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex32('w','i','r','e');

	p->oncreate = (void*)wire_create;
	p->ondelete = (void*)wire_delete;
	p->onsearch = (void*)wire_search;
	p->onmodify = (void*)wire_modify;

	p->onlinkup = (void*)wire_linkup;
	p->ondiscon = (void*)wire_discon;
	p->onread  = (void*)wire_read;
	p->onwrite = (void*)wire_write;
}
