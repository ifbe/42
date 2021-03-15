#include "libuser.h"
void gl41line_pmos(struct entity* wnd, u32 irgb, u32 orgb, vec3 vc, vec3 vr, vec3 vf, vec3 vt);
void gl41line_nmos(struct entity* wnd, u32 irgb, u32 orgb, vec3 vc, vec3 vr, vec3 vf, vec3 vt);




static void mem_draw_pixel(
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
static void mem_draw_gl41(
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

	//+
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vf[j];
		tr[j] = tc[j] -tf[j]/2;
	}
	gl41line(ctx, 0xff0000, tc,tr);

	//-
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vf[j];
		tr[j] = tc[j] +tf[j]/2;
	}
	gl41line(ctx, 0x0000ff, tc,tr);

	u32 lcolor[2] = {0xffffff, 0xffffff};
	u32 rcolor[2] = {0xffffff, 0xffffff};
	u32 gcolor[2];
	if(0 == act->ix0){lcolor[0] = 0xff0000;gcolor[0] = 0x0000ff;}
	else             {lcolor[1] = 0x0000ff;gcolor[0] = 0xff0000;}
	if(0 == act->iy0){rcolor[0] = 0xff0000;gcolor[1] = 0x0000ff;}
	else             {rcolor[1] = 0x0000ff;gcolor[1] = 0xff0000;}

	//l.p
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]/2 +vf[j]/2;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	gl41line_pmos(ctx, gcolor[0], lcolor[0], tc,tr,tf,vt);

	//l.n
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]/2 -vf[j]/2;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	gl41line_nmos(ctx, gcolor[0], lcolor[1], tc,tr,tf,vt);

	//l.g
	for(j=0;j<3;j++){
		tc[j] = vc[j] - vr[j]*3/4 - vf[j]/2;
		tr[j] = tc[j] + vf[j];
	}
	gl41line(ctx, gcolor[0], tc, tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] + vr[j]/2 - vf[j]/4;
		tr[j] = tc[j] + vf[j]/2;
	}
	gl41line(ctx, gcolor[0], tc, tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vf[j]/8 -vr[j]*3/4;
		tr[j] = vc[j] +vf[j]/8 +vr[j]/2;
	}
	gl41line(ctx, gcolor[0], tc, tr);

	//r.p
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]/2 +vf[j]/2;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	gl41line_pmos(ctx, gcolor[1], rcolor[0], tc,tr,tf,vt);

	//r.n
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]/2 -vf[j]/2;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	gl41line_nmos(ctx, gcolor[1], rcolor[1], tc,tr,tf,vt);

	//l.g
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]*1/4 -vf[j]/2;
		tr[j] = tc[j] +vf[j];
	}
	gl41line(ctx, gcolor[1], tc, tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]/2 -vf[j]/4;
		tr[j] = tc[j] +vf[j]/2;
	}
	gl41line(ctx, gcolor[1], tc, tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vf[j]/8 -vr[j]/2;
		tr[j] = vc[j] -vf[j]/8 +vr[j]*1/4;
	}
	gl41line(ctx, gcolor[1], tc, tr);
}
static void mem_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void mem_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void mem_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void mem_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void mem_wrl_cam_wnd(_ent* ent,void* slot, _syn* stack,int sp)
{
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	mem_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void mem_taking(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
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
	case _gl41full_:
		break;
	default:
		mem_wrl_cam_wnd(ent,foot, stack,sp);
		break;
	}
}
static void mem_giving(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, u8* buf,int len)
{
	u8 tmp;
	say("@memgate_write:%x\n",buf[0]);

	if('0' == buf[0])ent->ix0 = 0;
	else if('1' == buf[0])ent->ix0 = 1;
	else return;

	ent->iy0 = !ent->ix0;
}
static void mem_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void mem_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void mem_search(struct entity* act, u8* buf)
{
}
static void mem_modify(struct entity* act, u8* buf)
{
}
static void mem_delete(struct entity* act, u8* buf)
{
}
static void mem_create(struct entity* act, u8* buf)
{
	act->ix0 = getrandom()&1;
	act->iy0 = !act->ix0;
}




void mem_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex32('m','e','m', 0);

	p->oncreate = (void*)mem_create;
	p->ondelete = (void*)mem_delete;
	p->onsearch = (void*)mem_search;
	p->onmodify = (void*)mem_modify;

	p->onlinkup = (void*)mem_linkup;
	p->ondiscon = (void*)mem_discon;
	p->ontaking = (void*)mem_taking;
	p->ongiving = (void*)mem_giving;
}
