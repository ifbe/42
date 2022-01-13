#include "libuser.h"




static void gaafet_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void gaafet_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void gaafet_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void gaafet_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void gaafet_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void gaafet_draw_gl41(
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




static void gaafet_wrl_cam_wnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;

	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	gaafet_draw_gl41(ent, slot, wor,geom, wnd,area);
}
static void gaafet_wrl_wnd(_obj* ent,void* slot, _syn* stack,int sp)
{
}
static void gaafet_wnd(_obj* ent,void* slot, _syn* stack,int sp)
{
}




static void gaafet_taking(_obj* ent,void* slot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	//foot defined behavior
	switch(stack[sp-1].foottype){
	}

	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	//caller defined behavior
	switch(caller->hfmt){
	case _rgba_:
		break;
	case _gl41list_:
		gaafet_wnd(ent,slot, stack,sp);break;
		break;
	default:
		gaafet_wrl_cam_wnd(ent,slot, stack,sp);
	}
}
static void gaafet_giving(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void gaafet_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void gaafet_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void gaafet_search(_obj* act, u8* buf)
{
}
static void gaafet_modify(_obj* act, u8* buf)
{
}
static void gaafet_delete(_obj* act, u8* buf)
{
}
static void gaafet_create(_obj* act, void* arg, int argc, u8** argv)
{
	act->whdf.ix0 = 0;	//D
	act->whdf.iy0 = 0;	//S
	act->whdf.iz0 = 0;	//G
	act->whdf.iw0 = 0;	//B
}




void gaafet_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('g','a','a','f','e','t', 0, 0);

	p->oncreate = (void*)gaafet_create;
	p->ondelete = (void*)gaafet_delete;
	p->onsearch = (void*)gaafet_search;
	p->onmodify = (void*)gaafet_modify;

	p->onattach = (void*)gaafet_attach;
	p->ondetach = (void*)gaafet_detach;
	p->ontaking = (void*)gaafet_taking;
	p->ongiving = (void*)gaafet_giving;
}
