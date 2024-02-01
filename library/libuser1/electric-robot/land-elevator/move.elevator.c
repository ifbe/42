#include "libuser.h"




static void elevator_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void elevator_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void elevator_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void elevator_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void elevator_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void elevator_draw_gl41(
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




static void elevator_read_byworld_bycam_bywnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	elevator_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void elevator_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
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
		break;
	default:
		elevator_read_byworld_bycam_bywnd(ent,foot, stack,sp);
		break;
	}
}
static void elevator_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void elevator_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void elevator_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void elevator_search(_obj* act)
{
}
static void elevator_modify(_obj* act)
{
}
static void elevator_delete(_obj* act)
{
}
static void elevator_create(_obj* act)
{
}




void elevator_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex64('e','l','e','v','a','t','o','r');

	p->oncreate = (void*)elevator_create;
	p->ondelete = (void*)elevator_delete;
	p->onreader = (void*)elevator_search;
	p->onwriter = (void*)elevator_modify;

	p->onattach = (void*)elevator_attach;
	p->ondetach = (void*)elevator_detach;
	p->ontaking = (void*)elevator_taking;
	p->ongiving = (void*)elevator_giving;
}
