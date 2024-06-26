#include "libuser.h"




static void mosfet_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void mosfet_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void mosfet_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void mosfet_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void mosfet_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void mosfet_draw_gl41(
	_obj* act, struct style* slot,
	_obj* scn, struct style* geom,
	_obj* wnd, struct style* area)
{
	int j;
	vec3 tc,tr,tf,tu;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
	gl41line_rect(wnd, 0xffffff, vc,vr,vf);
}




static void mosfet_read_byworld_bycam_bywnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;

	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	mosfet_draw_gl41(ent, slot, wor,geom, wnd,area);
}
static void mosfet_read_byworld_bywnd(_obj* ent,void* slot, _syn* stack,int sp)
{
}
static void mosfet_read_bywnd(_obj* ent,void* slot, _syn* stack,int sp)
{
}




static void mosfet_taking(_obj* ent,void* slot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	//foot defined behavior
	switch(stack[sp-1].foottype){
	}

	//caller defined behavior
	switch(caller->type){
	case _wnd_:
	case _render_:
		mosfet_read_bywnd(ent,slot, stack,sp);
		break;
	default:
		mosfet_read_byworld_bycam_bywnd(ent,slot, stack,sp);
	}
}
static void mosfet_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void mosfet_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void mosfet_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void mosfet_search(_obj* act, u8* buf)
{
}
static void mosfet_modify(_obj* act, u8* buf)
{
}
static void mosfet_delete(_obj* act, u8* buf)
{
}
static void mosfet_create(_obj* act, void* arg, int argc, u8** argv)
{
	act->whdf.ix0 = 0;	//D
	act->whdf.iy0 = 0;	//S
	act->whdf.iz0 = 0;	//G
	act->whdf.iw0 = 0;	//B
}




void mosfet_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex64('m','o','s','f','e','t', 0, 0);

	p->oncreate = (void*)mosfet_create;
	p->ondelete = (void*)mosfet_delete;
	p->onreader = (void*)mosfet_search;
	p->onwriter = (void*)mosfet_modify;

	p->onattach = (void*)mosfet_attach;
	p->ondetach = (void*)mosfet_detach;
	p->ontaking = (void*)mosfet_taking;
	p->ongiving = (void*)mosfet_giving;
}
