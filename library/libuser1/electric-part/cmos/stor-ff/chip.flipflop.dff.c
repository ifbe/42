#include "libuser.h"
void gl41line_pmos(_obj* wnd, u32 irgb, u32 orgb, vec3 vc, vec3 vr, vec3 vf, vec3 vt);
void gl41line_nmos(_obj* wnd, u32 irgb, u32 orgb, vec3 vc, vec3 vr, vec3 vf, vec3 vt);




static void dff_draw_pixel(
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
static void dff_draw_gl41(
	_obj* act, struct style* slot,
	_obj* win, struct style* geom,
	_obj* ctx, struct style* area)
{
	int j;
	vec3 tc,tr,tf,tt;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
	//gl41line_rect(ctx, 0x404040, vc, vr, vf);

	//vcc
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j] +vf[j];
		tr[j] = vc[j] +vr[j] +vf[j];
	}
	gl41line(ctx, 0xff0000, tc, tr);

	//gnd
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j] -vf[j];
		tr[j] = vc[j] +vr[j] -vf[j];
	}
	gl41line(ctx, 0x0000ff, tc, tr);

	//d, ck', ck, d
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]/2 +vf[j]*3/4;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	gl41line_pmos(ctx, 0xffffff, 0xffffff, tc,tr,tf,vt);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]/2 +vf[j]*1/4;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	gl41line_pmos(ctx, 0xffffff, 0xffffff, tc,tr,tf,vt);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]/2 -vf[j]*1/4;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	gl41line_nmos(ctx, 0xffffff, 0xffffff, tc,tr,tf,vt);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]/2 -vf[j]*3/4;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	gl41line_nmos(ctx, 0xffffff, 0xffffff, tc,tr,tf,vt);

	//not
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vf[j]*1/4;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	gl41line_pmos(ctx, 0xffffff, 0xffffff, tc,tr,tf,vt);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vf[j]*1/4;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	gl41line_nmos(ctx, 0xffffff, 0xffffff, tc,tr,tf,vt);

	//q, ck', ck, q
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]/2 +vf[j]*3/4;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	gl41line_pmos(ctx, 0xffffff, 0xffffff, tc,tr,tf,vt);
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]/2 +vf[j]*1/4;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	gl41line_pmos(ctx, 0xffffff, 0xffffff, tc,tr,tf,vt);
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]/2 -vf[j]*1/4;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	gl41line_nmos(ctx, 0xffffff, 0xffffff, tc,tr,tf,vt);
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]/2 -vf[j]*3/4;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	gl41line_nmos(ctx, 0xffffff, 0xffffff, tc,tr,tf,vt);

	//d: shu
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]*7/8 + vf[j]*3/4;
		tr[j] = tc[j] -vf[j]*3/2;
	}
	gl41line(ctx, 0xffffff, tc, tr);

	//q': heng, shu
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]/2;
		tr[j] = tc[j] +vr[j]/4;
	}
	gl41line(ctx, 0xffffff, tc, tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]*2/8 + vf[j]/4;
		tr[j] = tc[j] -vf[j]/2;
	}
	gl41line(ctx, 0xffffff, tc, tr);

	//q: heng, shu
	for(j=0;j<3;j++){
		tc[j] = vc[j];
		tr[j] = tc[j] +vr[j]/8;
	}
	gl41line(ctx, 0xffffff, tc, tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]*1/8 + vf[j]*3/4;
		tr[j] = tc[j] -vf[j]*3/2;
	}
	gl41line(ctx, 0xffffff, tc, tr);

	//q': heng, shu, back, shu
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]/2;
		tr[j] = tc[j] +vr[j]/4;
	}
	gl41line(ctx, 0xffffff, tc, tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]*3/8;
		tr[j] = tc[j] -vf[j]*7/8;
	}
	gl41line(ctx, 0xffffff, tc, tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]*3/8 -vf[j]*7/8;
		tr[j] = tc[j] +vr[j]*9/8;
	}
	gl41line(ctx, 0xffffff, tc, tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]*6/8;
		tr[j] = tc[j] -vf[j]*7/8;
	}
	gl41line(ctx, 0xffffff, tc, tr);
}
static void dff_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void dff_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void dff_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void dff_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void dff_read_byworld_bycam_bywnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	dff_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void dff_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
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
		dff_read_byworld_bycam_bywnd(ent,foot, stack,sp);
		break;
	}
}
static void dff_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void dff_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void dff_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void dff_search(_obj* act, u8* buf)
{
}
static void dff_modify(_obj* act, u8* buf)
{
}
static void dff_delete(_obj* act, u8* buf)
{
}
static void dff_create(_obj* act, u8* buf)
{
}




void dff_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex32('d','f','f', 0);

	p->oncreate = (void*)dff_create;
	p->ondelete = (void*)dff_delete;
	p->onreader = (void*)dff_search;
	p->onwriter = (void*)dff_modify;

	p->onattach = (void*)dff_attach;
	p->ondetach = (void*)dff_detach;
	p->ontaking = (void*)dff_taking;
	p->ongiving = (void*)dff_giving;
}
