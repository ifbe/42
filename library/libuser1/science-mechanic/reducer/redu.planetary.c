#include "libuser.h"




static void planetary_search(_obj* act, u8* buf)
{
}
static void planetary_modify(_obj* act, u8* buf)
{
}
static void planetary_delete(_obj* act, u8* buf)
{
}
static void planetary_create(_obj* act, u8* buf)
{
}




static void planetary_draw_gl41(
	_obj* act, struct style* part,
	_obj* win, struct style* geom,
	_obj* ctx, struct style* area)
{
	int j;
	float r,a,c,s;
	vec3 tc,tr,tf,tt;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;

	r = tau*(timeread_us()%10000000)/10000000.0;
	gl41line_rotategear(ctx, 0x444444, vc, vr, vf, 48, r*18/48.0);

	for(j=0;j<3;j++){
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	gl41solid_rotategear(ctx, 0x808080, vc, tr, tf, vt, 12, -r*18/12.0+tau/24);

	//planet 0
	for(j=0;j<3;j++){
		tc[j] = vc[j] + vr[j]*0.575;
		tr[j] = vr[j]*0.35;
		tf[j] = vf[j]*0.35;
	}
	gl41solid_rotategear(ctx, 0x808080, tc, tr, tf, vt, 18, r);

	//planet 1
	a = 120*PI/180;
	c = getcos(a)*0.575;
	s = getsin(a)*0.575;
	for(j=0;j<3;j++)tc[j] = vc[j] + vr[j]*c + vf[j]*s;
	gl41solid_rotategear(ctx, 0x808080, tc, tr, tf, vt, 18, r);

	//planet 2
	a = 240*PI/180;
	c = getcos(a)*0.575;
	s = getsin(a)*0.575;
	for(j=0;j<3;j++)tc[j] = vc[j] + vr[j]*c + vf[j]*s;
	gl41solid_rotategear(ctx, 0x808080, tc, tr, tf, vt, 18, r);
}
static void planetary_draw_pixel(
	_obj* act, struct style* pin,
	_obj* wnd, struct style* sty)
{
}
static void planetary_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void planetary_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void planetary_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void planetary_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void planetary_wrl_cam_wnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	planetary_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void planetary_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
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
		planetary_wrl_cam_wnd(ent,foot, stack,sp);
		break;
	}
}
static void planetary_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void planetary_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void planetary_attach(struct halfrel* self, struct halfrel* peer)
{
}




void planetary_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('p','l','a','n','e','t','a','r');

	p->oncreate = (void*)planetary_create;
	p->ondelete = (void*)planetary_delete;
	p->onreader = (void*)planetary_search;
	p->onwriter = (void*)planetary_modify;

	p->onattach = (void*)planetary_attach;
	p->ondetach = (void*)planetary_detach;
	p->ontaking = (void*)planetary_taking;
	p->ongiving = (void*)planetary_giving;
}
