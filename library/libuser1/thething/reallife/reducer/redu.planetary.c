#include "libuser.h"




static void planetary_search(struct entity* act, u8* buf)
{
}
static void planetary_modify(struct entity* act, u8* buf)
{
}
static void planetary_delete(struct entity* act, u8* buf)
{
}
static void planetary_create(struct entity* act, u8* buf)
{
}




static void planetary_draw_gl41(
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	int j;
	float r,a,c,s;
	vec3 tc,tr,tf,tt;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;

	r = tau*(timeread()%10000000)/10000000.0;
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
	struct entity* act, struct style* pin,
	struct entity* wnd, struct style* sty)
{
}
static void planetary_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void planetary_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void planetary_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void planetary_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void planetary_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	if(stack&&('v' == key)){
		slot = stack[sp-1].pfoot;
		wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		planetary_draw_gl41(ent,slot, wor,geom, wnd,area);
	}
}
static void planetary_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void planetary_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void planetary_linkup(struct halfrel* self, struct halfrel* peer)
{
}




void planetary_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('p','l','a','n','e','t','a','r');

	p->oncreate = (void*)planetary_create;
	p->ondelete = (void*)planetary_delete;
	p->onsearch = (void*)planetary_search;
	p->onmodify = (void*)planetary_modify;

	p->onlinkup = (void*)planetary_linkup;
	p->ondiscon = (void*)planetary_discon;
	p->ontaking = (void*)planetary_taking;
	p->ongiving = (void*)planetary_giving;
}
