#include "libuser.h"




static void and_draw_pixel(
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
static void and_draw_gl41(
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
static void and_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void and_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void and_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void and_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void and_wrl_cam_wnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	and_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void and_taking(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
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
		and_wrl_cam_wnd(ent,foot, stack,sp);
		break;
	}
}
static void and_giving(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void and_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void and_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void and_search(_obj* act, u8* buf)
{
}
static void and_modify(_obj* act, u8* buf)
{
}
static void and_delete(_obj* act, u8* buf)
{
}
static void and_create(_obj* act, u8* buf)
{
}




void and_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex32('a','n','d', 0);

	p->oncreate = (void*)and_create;
	p->ondelete = (void*)and_delete;
	p->onsearch = (void*)and_search;
	p->onmodify = (void*)and_modify;

	p->onlinkup = (void*)and_linkup;
	p->ondiscon = (void*)and_discon;
	p->ontaking = (void*)and_taking;
	p->ongiving = (void*)and_giving;
}
