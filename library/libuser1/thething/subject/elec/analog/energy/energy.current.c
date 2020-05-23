#include "libuser.h"




static void isrc_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void isrc_draw_gl41(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void isrc_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void isrc_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void isrc_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void isrc_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void isrc_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void isrc_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void isrc_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void isrc_linkup(struct halfrel* self, struct halfrel* peer)
{
}





static void isrc_search(struct entity* act, u8* buf)
{
}
static void isrc_modify(struct entity* act, u8* buf)
{
}
static void isrc_delete(struct entity* act, u8* buf)
{
}
static void isrc_create(struct entity* act, u8* buf)
{
}




void isrc_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex32('i','s','r','c');

	p->oncreate = (void*)isrc_create;
	p->ondelete = (void*)isrc_delete;
	p->onsearch = (void*)isrc_search;
	p->onmodify = (void*)isrc_modify;

	p->onlinkup = (void*)isrc_linkup;
	p->ondiscon = (void*)isrc_discon;
	p->ontaking = (void*)isrc_taking;
	p->ongiving = (void*)isrc_giving;
}
