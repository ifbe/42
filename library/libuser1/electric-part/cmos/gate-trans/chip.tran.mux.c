#include "libuser.h"




static void mux_draw_pixel(
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
static void mux_draw_gl41(
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
static void mux_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void mux_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void mux_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void mux_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void mux_read_byworld_bycam_bywnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	mux_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void mux_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
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
		mux_read_byworld_bycam_bywnd(ent,foot, stack,sp);
		break;
	}
}
static void mux_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void mux_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void mux_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void mux_search(_obj* act, u8* buf)
{
}
static void mux_modify(_obj* act, u8* buf)
{
}
static void mux_delete(_obj* act, u8* buf)
{
}
static void mux_create(_obj* act, u8* buf)
{
}




void mux_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex32('m','u','x', 0);

	p->oncreate = (void*)mux_create;
	p->ondelete = (void*)mux_delete;
	p->onreader = (void*)mux_search;
	p->onwriter = (void*)mux_modify;

	p->onattach = (void*)mux_attach;
	p->ondetach = (void*)mux_detach;
	p->ontaking = (void*)mux_taking;
	p->ongiving = (void*)mux_giving;
}
