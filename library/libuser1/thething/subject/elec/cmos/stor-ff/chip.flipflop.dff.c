#include "libuser.h"
void gl41line_pmos(struct entity* wnd, u32 irgb, u32 orgb, vec3 vc, vec3 vr, vec3 vf, vec3 vt);
void gl41line_nmos(struct entity* wnd, u32 irgb, u32 orgb, vec3 vc, vec3 vr, vec3 vf, vec3 vt);




static void dff_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
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
		cx = win->width/2;
		cy = win->height/2;
		ww = win->width/2;
		hh = win->height/2;
	}
}
static void dff_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
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
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void dff_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void dff_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void dff_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void dff_wrl_cam_wnd(_ent* ent,void* slot, _syn* stack,int sp)
{
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	dff_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void dff_taking(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	//foot defined behavior
	switch(stack[sp-1].flag){
	}

	//caller defined behavior
	struct entity* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(caller->fmt){
	case _rgba_:
		break;
	case _gl41list_:
		break;
	default:
		dff_wrl_cam_wnd(ent,foot, stack,sp);
		break;
	}
}
static void dff_giving(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void dff_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void dff_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void dff_search(struct entity* act, u8* buf)
{
}
static void dff_modify(struct entity* act, u8* buf)
{
}
static void dff_delete(struct entity* act, u8* buf)
{
}
static void dff_create(struct entity* act, u8* buf)
{
}




void dff_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex32('d','f','f', 0);

	p->oncreate = (void*)dff_create;
	p->ondelete = (void*)dff_delete;
	p->onsearch = (void*)dff_search;
	p->onmodify = (void*)dff_modify;

	p->onlinkup = (void*)dff_linkup;
	p->ondiscon = (void*)dff_discon;
	p->ontaking = (void*)dff_taking;
	p->ongiving = (void*)dff_giving;
}
