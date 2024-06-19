#include "libuser.h"




static void telpher_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void telpher_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void telpher_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void telpher_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void telpher_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void telpher_draw_gl41(
	_obj* act, struct style* slot,
	_obj* scn, struct style* geom,
	_obj* wnd, struct style* area)
{
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
	gl41line_prism4(wnd, 0xffffff, vc, vr, vf, vt);
}
static void telpher_read_byworld_bycam_bywnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	telpher_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void telpher_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	//foot defined behavior
	switch(stack[sp-1].foottype){
	}

	//caller defined behavior
	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(caller->type){
	case _wnd_:
	case _render_:
		break;
	default:
		telpher_read_byworld_bycam_bywnd(ent,foot, stack,sp);
		break;
	}
}
static void telpher_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void telpher_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void telpher_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void telpher_search(_obj* act)
{
}
static void telpher_modify(_obj* act)
{
}
static void telpher_delete(_obj* act)
{
}
static void telpher_create(_obj* act)
{
}




void telpher_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex64('t','e','l','p','h','e','r', 0);

	p->oncreate = (void*)telpher_create;
	p->ondelete = (void*)telpher_delete;
	p->onreader = (void*)telpher_search;
	p->onwriter = (void*)telpher_modify;

	p->onattach = (void*)telpher_attach;
	p->ondetach = (void*)telpher_detach;
	p->ontaking = (void*)telpher_taking;
	p->ongiving = (void*)telpher_giving;
}
