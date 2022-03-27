#include "libuser.h"




static void isrc_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void isrc_draw_gl41(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void isrc_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void isrc_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void isrc_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void isrc_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void isrc_taking(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void isrc_giving(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void isrc_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void isrc_attach(struct halfrel* self, struct halfrel* peer)
{
}





static void isrc_search(_obj* act, u8* buf)
{
}
static void isrc_modify(_obj* act, u8* buf)
{
}
static void isrc_delete(_obj* act, u8* buf)
{
}
static void isrc_create(_obj* act, u8* buf)
{
}




void isrc_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex32('i','s','r','c');

	p->oncreate = (void*)isrc_create;
	p->ondelete = (void*)isrc_delete;
	p->onreader = (void*)isrc_search;
	p->onwriter = (void*)isrc_modify;

	p->onattach = (void*)isrc_attach;
	p->ondetach = (void*)isrc_detach;
	p->ontaking = (void*)isrc_taking;
	p->ongiving = (void*)isrc_giving;
}
