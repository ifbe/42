#include "libuser.h"




static void or_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	int cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->fs.vc[0];
		cy = sty->fs.vc[1];
		ww = sty->fs.vr[0];
		hh = sty->fs.vf[1];
	}
	else
	{
		cx = win->whdf.width/2;
		cy = win->whdf.height/2;
		ww = win->whdf.width/2;
		hh = win->whdf.height/2;
	}
}
static void or_draw_gl41(
	_obj* act, struct style* slot,
	_obj* win, struct style* geom,
	_obj* ctx, struct style* area)
{
	vec3 tc,tr,tf,tu;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;
	gl41line_prism4(ctx, 0xffffff, vc, vr, vf, vu);
}
static void or_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void or_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void or_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void or_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void or_wrl_cam_wnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	or_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void or_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	//foot defined behavior
	switch(stack[sp-1].foottype){
	}

	//caller defined behavior
	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(caller->hfmt){
	case _rgba_:
		break;
	case _gl41list_:
		break;
	default:
		or_wrl_cam_wnd(ent,foot, stack,sp);
		break;
	}
}
static void or_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void or_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void or_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void or_search(_obj* act, u8* buf)
{
}
static void or_modify(_obj* act, u8* buf)
{
}
static void or_delete(_obj* act, u8* buf)
{
}
static void or_create(_obj* act, u8* buf)
{
}




void or_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex32('o','r', 0, 0);

	p->oncreate = (void*)or_create;
	p->ondelete = (void*)or_delete;
	p->onreader = (void*)or_search;
	p->onwriter = (void*)or_modify;

	p->onattach = (void*)or_attach;
	p->ondetach = (void*)or_detach;
	p->ontaking = (void*)or_taking;
	p->ongiving = (void*)or_giving;
}
