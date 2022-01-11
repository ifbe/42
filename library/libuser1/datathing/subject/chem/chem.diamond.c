#include "libuser.h"




static void diamond_search(_obj* act, u8* buf)
{
}
static void diamond_modify(_obj* act, u8* buf)
{
}
static void diamond_delete(_obj* act, u8* buf)
{
}
static void diamond_create(_obj* act, u8* buf)
{
}




static void diamond_draw_gl41(
	_obj* act, struct style* part,
	_obj* win, struct style* geom,
	_obj* ctx, struct style* area)
{
	int j;
	vec3 tc,tr,tf,tt;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
	gl41line_prism4(ctx, 0x444444, vc, vr, vf, vt);

	for(j=0;j<3;j++){
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
		tt[j] = vt[j]/4;
	}
	gl41solid_sphere(ctx, 0x808080, vc, tr, tf, tt);
}
static void diamond_draw_pixel(
	_obj* act, struct style* pin,
	_obj* wnd, struct style* sty)
{
}
static void diamond_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void diamond_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void diamond_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void diamond_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void diamond_wrl_cam_wnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	diamond_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void diamond_taking(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
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
		diamond_wrl_cam_wnd(ent,foot, stack,sp);
		break;
	}
}
static void diamond_giving(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void diamond_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void diamond_linkup(struct halfrel* self, struct halfrel* peer)
{
}




void diamond_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('d','i','a','m','o','n','d', 0);

	p->oncreate = (void*)diamond_create;
	p->ondelete = (void*)diamond_delete;
	p->onsearch = (void*)diamond_search;
	p->onmodify = (void*)diamond_modify;

	p->onlinkup = (void*)diamond_linkup;
	p->ondiscon = (void*)diamond_discon;
	p->ontaking = (void*)diamond_taking;
	p->ongiving = (void*)diamond_giving;
}
