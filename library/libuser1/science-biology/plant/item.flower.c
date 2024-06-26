#include "libuser.h"




static void flower_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void flower_draw_gl41(
	_obj* act, struct style* part,
	_obj* win, struct style* geom,
	_obj* ctx, struct style* area)
{
	int j;
	float len;
	vec3 tc,tr,tf,tt;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;

	for(j=0;j<3;j++){
		tc[j] = vc[j] + vt[j]*9/20;
		tr[j] = vr[j]/8;
		tf[j] = vf[j]/8;
		tt[j] = vt[j]*9/20;
	}
	gl41solid_cylinder(ctx, 0x00ff00, tc,tr,tf,tt);

	len = vec3_getlen(tr);
	//vec3_setlen(tr, len);
	vec3_setlen(tf, len);
	vec3_setlen(tt, len);
	for(j=0;j<3;j++){
		tc[j] = vc[j] + vt[j]*19/20;
	}
	gl41solid_sphere(ctx, 0xffff00, tc,tr,tf,tt);

	for(j=0;j<3;j++){
		tc[j] = vc[j] + vr[j]/2 + vt[j]*19/20;
		tr[j] = vr[j]/2 + vt[j]/20;
		tf[j] = vf[j]/2;
	}
	for(j=0;j<6;j++){
		gl41solid_circle(ctx, 0xff0000, tc,tr,tf);
		quaternion_operation(tc, vt, PI/3);
		quaternion_operation(tr, vt, PI/3);
		quaternion_operation(tf, vt, PI/3);
	}
}
static void flower_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void flower_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void flower_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void flower_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void flower_read_byworld_bycam_bywnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	flower_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void flower_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
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
		flower_read_byworld_bycam_bywnd(ent,foot, stack,sp);
		break;
	}
}
static void flower_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void flower_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void flower_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void flower_search(_obj* act)
{
}
static void flower_modify(_obj* act)
{
}
static void flower_delete(_obj* act)
{
}
static void flower_create(_obj* act)
{
}




void flower_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex64('f','l','o','w','e','r', 0, 0);

	p->oncreate = (void*)flower_create;
	p->ondelete = (void*)flower_delete;
	p->onreader = (void*)flower_search;
	p->onwriter = (void*)flower_modify;

	p->onattach = (void*)flower_attach;
	p->ondetach = (void*)flower_detach;
	p->ontaking = (void*)flower_taking;
	p->ongiving = (void*)flower_giving;
}
