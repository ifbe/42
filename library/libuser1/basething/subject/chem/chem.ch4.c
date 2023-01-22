#include "libuser.h"




static void ch4_search(_obj* act, u8* buf)
{
}
static void ch4_modify(_obj* act, u8* buf)
{
}
static void ch4_delete(_obj* act, u8* buf)
{
}
static void ch4_create(_obj* act, u8* buf)
{
}




static void ch4_draw_gl41(
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

	//carbon
	for(j=0;j<3;j++){
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
		tt[j] = vt[j]/4;
	}
	gl41solid_sphere(ctx, 0x808080, vc, tr, tf, tt);

	for(j=0;j<3;j++){
		tr[j] = vr[j]/8;
		tf[j] = vf[j]/8;
		tt[j] = vt[j]/8;
		tc[j] = vc[j] +vt[j]-tt[j];
	}
	gl41solid_sphere(ctx, 0x808080, tc, tr, tf, tt);
}
static void ch4_draw_pixel(
	_obj* act, struct style* pin,
	_obj* wnd, struct style* sty)
{
}
static void ch4_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void ch4_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void ch4_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void ch4_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void ch4_wrl_cam_wnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	ch4_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void ch4_taking(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
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
		ch4_wrl_cam_wnd(ent,foot, stack,sp);
		break;
	}
}
static void ch4_giving(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void ch4_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void ch4_attach(struct halfrel* self, struct halfrel* peer)
{
}




void ch4_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex32('c','h','4', 0);

	p->oncreate = (void*)ch4_create;
	p->ondelete = (void*)ch4_delete;
	p->onreader = (void*)ch4_search;
	p->onwriter = (void*)ch4_modify;

	p->onattach = (void*)ch4_attach;
	p->ondetach = (void*)ch4_detach;
	p->ontaking = (void*)ch4_taking;
	p->ongiving = (void*)ch4_giving;
}
