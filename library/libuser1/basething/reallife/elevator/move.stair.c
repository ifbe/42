#include "libuser.h"




static void stair_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void stair_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void stair_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void stair_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void stair_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void stair_draw_gl41(
	_obj* act, struct style* slot,
	_obj* scn, struct style* geom,
	_obj* wnd, struct style* area)
{
	int z,j;
	vec3 tc,tr,tf,tt;
	vec3 kc,kr,kf,kt;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
	//gl41line_prism4(wnd, 0xffffff, vc, vr, vf, vt);

	for(j=0;j<3;j++){
		tr[j] = vr[j]/8;
		tf[j] = vf[j]/8;
		tt[j] = vt[j]/64;
	}
	for(z=0;z<8;z++)
	{
		for(j=0;j<3;j++)tc[j] = vc[j] + tf[j]*(z*2-7) + vt[j]*(z+1)/9-tt[j];
		gl41solid_prism4(wnd, 0x808080, tc, vr, tf, tt);
	}

	for(j=0;j<3;j++){
		kr[j] = vr[j]/64;
		kf[j] = vf[j]/64;
		kt[j] = vt[j]/18;
	}
	for(z=0;z<=8;z++)
	{
		for(j=0;j<3;j++)kc[j] = vc[j] -vr[j]+kr[j] + tf[j]*(z*2-8) + vt[j]*(z+1)/9-kt[j];
		gl41solid_cylinder(wnd, 0x800000, kc, kr, kf, kt);

		for(j=0;j<3;j++)kc[j] = vc[j] +vr[j]-kr[j] + tf[j]*(z*2-8) + vt[j]*(z+1)/9-kt[j];
		gl41solid_cylinder(wnd, 0x800000, kc, kr, kf, kt);
	}
}




static void stair_wrl_cam_wnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	stair_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void stair_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
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
		stair_wrl_cam_wnd(ent,foot, stack,sp);
		break;
	}
}
static void stair_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void stair_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void stair_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void stair_search(_obj* act)
{
}
static void stair_modify(_obj* act)
{
}
static void stair_delete(_obj* act)
{
	if(0 == act)return;
	//if(_copy_ == act->type)memorydelete(act->buf);
}
static void stair_create(_obj* act)
{
	if(0 == act)return;
	//if(_orig_ == act->type)act->buf = buffer;
	//if(_copy_ == act->type)act->buf = memorycreate(256, 0);
}




void stair_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('s', 't', 'a', 'i', 'r', 0, 0, 0);

	p->oncreate = (void*)stair_create;
	p->ondelete = (void*)stair_delete;
	p->onreader = (void*)stair_search;
	p->onwriter = (void*)stair_modify;

	p->onattach = (void*)stair_attach;
	p->ondetach = (void*)stair_detach;
	p->ontaking = (void*)stair_taking;
	p->ongiving = (void*)stair_giving;
}
