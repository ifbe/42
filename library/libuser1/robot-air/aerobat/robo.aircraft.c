#include "libuser.h"
#define _euler_ hex32('e','u','l','r')
#define _quat_ hex32('q','u','a','t')
#define _real_ hex32('r','e','a','l')
#define _imag_ hex32('i','m','a','g')




static void aircraft_forgl41(
	_obj* act, struct style* part,
	_obj* scn, struct style* geom,
	_obj* ctx, struct style* area)
{
	float* vc = geom->fshape.vc;
	float* vr = geom->fshape.vr;
	float* vf = geom->fshape.vf;
	float* vt = geom->fshape.vt;
	gl41line_rect(ctx, 0x00ff00, vc, vr, vf);
	gl41line_prism4(ctx, 0x00ff00, vc, vr, vf, vt);

	int j;
	vec3 tc,tr,tf,tt;

	//tail
	for(j=0;j<3;j++){
		tc[j] = vc[j] - vf[j]*7/8 + vt[j]*3/4;
		tr[j] = vr[j]/32;
		tf[j] = vf[j]/8;
		tt[j] = vt[j]/4;
	}
	gl41solid_prism4(ctx, 0x404040, tc,tr,tf,tt);

	//wing
	for(j=0;j<3;j++){
		tc[j] = vc[j] + vt[j]/2;
		tr[j] = vr[j];
		tf[j] = vf[j]/8;
		tt[j] = vt[j]/32;
	}
	gl41solid_prism4(ctx, 0x404040, tc,tr,tf,tt);

	//body
	for(j=0;j<3;j++){
		tc[j] = vc[j] + vt[j]*3/8;
		tr[j] = vr[j]/8;
		tf[j] =-vt[j]/8;
		tt[j] = vf[j];
	}
	gl41solid_cylinder(ctx, 0x404040, tc, tr, tf, tt);

	//wheel
	for(j=0;j<3;j++){
		tc[j] = vc[j] + vf[j]/2 + vt[j]*1/8;
		tr[j] = vf[j]/8;
		tf[j] = vt[j]/8;
		tt[j] = vr[j]/32;
	}
	gl41solid_cylinder(ctx, 0x404040, tc, tr, tf, tt);		//front
	for(j=0;j<3;j++){
		tc[j] = vc[j] + vr[j]/16 - vf[j]/4 + vt[j]*1/8;
	}
	gl41solid_cylinder(ctx, 0x404040, tc, tr, tf, tt);		//rear left
	for(j=0;j<3;j++){
		tc[j] = vc[j] - vr[j]/16 - vf[j]/4 + vt[j]*1/8;
	}
	gl41solid_cylinder(ctx, 0x404040, tc, tr, tf, tt);		//rear right
}
static void aircraft_draw_pixel(
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
static void aircraft_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void aircraft_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void aircraft_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void aircraft_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void aircraft_read_byworld_bycam_bywnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor = stack[sp-2].pchip;
	struct style* geom = stack[sp-2].pfoot;
	_obj* wnd = stack[sp-6].pchip;
	struct style* area = stack[sp-6].pfoot;
	aircraft_forgl41(ent,slot, wor,geom, wnd,area);
}




static void aircraft_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	//caller defined behavior
	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(caller->type){
	case _wnd_:
	case _render_:
		break;
	default:
		aircraft_read_byworld_bycam_bywnd(ent,foot, stack,sp);
		break;
	}
}
static void aircraft_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void aircraft_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void aircraft_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void aircraft_modify(_obj* act)
{
}
static void aircraft_search(_obj* act)
{
}
static void aircraft_delete(_obj* act)
{
}
static void aircraft_create(_obj* act)
{
}




void aircraft_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex64('a', 'i', 'r', 'c', 'r', 'a', 'f', 't');

	p->oncreate = (void*)aircraft_create;
	p->ondelete = (void*)aircraft_delete;
	p->onreader = (void*)aircraft_search;
	p->onwriter = (void*)aircraft_modify;

	p->onattach = (void*)aircraft_attach;
	p->ondetach = (void*)aircraft_detach;
	p->ontaking = (void*)aircraft_taking;
	p->ongiving = (void*)aircraft_giving;
}
