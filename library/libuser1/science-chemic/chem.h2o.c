#include "libuser.h"




static void h2o_search(_obj* act, u8* buf)
{
}
static void h2o_modify(_obj* act, u8* buf)
{
}
static void h2o_delete(_obj* act, u8* buf)
{
}
static void h2o_create(_obj* act, u8* buf)
{
}




static void h2o_draw_gl41(
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

	//oxygen
	for(j=0;j<3;j++){
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
		tt[j] = vt[j]/4;
	}
	gl41solid_sphere(ctx, 0x808080, vc, tr, tf, tt);

	//hydrogen
	for(j=0;j<3;j++){
		tr[j] = vr[j]/8;
		tf[j] = vf[j]/8;
		tt[j] = vt[j]/8;
		tc[j] = vc[j] +vr[j]-tr[j];
	}
	gl41solid_sphere(ctx, 0x808080, tc, tr, tf, tt);

	//hydrogen
	for(j=0;j<3;j++)tc[j] = vr[j]-tr[j];
	quaternion_operation(tc, vt, PI*(104+28.0/60)/180);
	for(j=0;j<3;j++)tc[j] += vc[j];
	gl41solid_sphere(ctx, 0x808080, tc, tr, tf, tt);
}
static void h2o_draw_pixel(
	_obj* act, struct style* pin,
	_obj* wnd, struct style* sty)
{
}
static void h2o_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void h2o_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void h2o_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void h2o_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void h2o_read_byworld_bycam_bywnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	h2o_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void h2o_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
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
		h2o_read_byworld_bycam_bywnd(ent,foot, stack,sp);
		break;
	}
}
static void h2o_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void h2o_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void h2o_attach(struct halfrel* self, struct halfrel* peer)
{
}




void h2o_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex32('h','2','o', 0);

	p->oncreate = (void*)h2o_create;
	p->ondelete = (void*)h2o_delete;
	p->onreader = (void*)h2o_search;
	p->onwriter = (void*)h2o_modify;

	p->onattach = (void*)h2o_attach;
	p->ondetach = (void*)h2o_detach;
	p->ontaking = (void*)h2o_taking;
	p->ongiving = (void*)h2o_giving;
}
