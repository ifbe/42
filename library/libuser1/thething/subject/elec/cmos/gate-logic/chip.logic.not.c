#include "libuser.h"
void gl41line_pmos(struct entity* wnd, u32 irgb, u32 orgb, vec3 vc, vec3 vr, vec3 vf, vec3 vt);
void gl41line_nmos(struct entity* wnd, u32 irgb, u32 orgb, vec3 vc, vec3 vr, vec3 vf, vec3 vt);




static void not_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	int cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->f.vc[0];
		cy = sty->f.vc[1];
		ww = sty->f.vr[0];
		hh = sty->f.vf[1];
	}
	else
	{
		cx = win->width/2;
		cy = win->height/2;
		ww = win->width/2;
		hh = win->height/2;
	}
}
static void not_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	int j;
	vec3 tc,tr,tf,tt;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vt = geom->f.vt;
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

	u32 pcolor = act->ix0 ? 0xffffff : 0xff0000;
	u32 ncolor = act->ix0 ? 0x0000ff : 0xffffff;
	u32 icolor = act->ix0 ? 0xff0000 : 0x0000ff;
	u32 ocolor = act->iy0 ? 0xff0000 : 0x0000ff;

	//p
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vf[j]/2;
		tr[j] = vr[j]/2;
		tf[j] = vf[j]/2;
	}
	gl41line_pmos(ctx, icolor, pcolor, tc,tr,tf,vt);

	//n
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vf[j]/2;
		tr[j] = vr[j]/2;
		tf[j] = vf[j]/2;
	}
	gl41line_nmos(ctx, icolor, ncolor, tc,tr,tf,vt);

	//i
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]/2 -vf[j]/2;
		tr[j] = tc[j] +vf[j];
	}
	gl41line(ctx, icolor, tc,tr);

	//o
	for(j=0;j<3;j++)tc[j] = vc[j]+vr[j];
	gl41line(ctx, ocolor, vc,tc);
}
static void not_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void not_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void not_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void not_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void not_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	if(stack&&('v' == key)){
		slot = stack[sp-1].pfoot;
		wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		not_draw_gl41(ent,slot, wor,geom, wnd,area);
	}
}
static void not_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, u8* buf,int len)
{
	u8 tmp;
	say("@notgate_write:%x\n",buf[0]);

	if('a' == buf[0])ent->ix0 ^= 1;
	else if('0' == buf[0])ent->ix0 = 0;
	else if('1' == buf[0])ent->ix0 = 1;
	else return;

	ent->iy0 = !ent->ix0;
	tmp = ent->iy0 + 0x30;
	give_data_into_peer(ent,'o', stack,sp, 0,0, &tmp,1);
}
static void not_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void not_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void not_search(struct entity* act, u8* buf)
{
}
static void not_modify(struct entity* act, u8* buf)
{
}
static void not_delete(struct entity* act, u8* buf)
{
}
static void not_create(struct entity* act, u8* buf)
{
	act->ix0 = getrandom()&1;
	act->iy0 = !act->ix0;
}




void not_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex32('n','o','t', 0);

	p->oncreate = (void*)not_create;
	p->ondelete = (void*)not_delete;
	p->onsearch = (void*)not_search;
	p->onmodify = (void*)not_modify;

	p->onlinkup = (void*)not_linkup;
	p->ondiscon = (void*)not_discon;
	p->ontaking = (void*)not_taking;
	p->ongiving = (void*)not_giving;
}
