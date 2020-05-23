#include "libuser.h"




static void h2o_search(struct entity* act, u8* buf)
{
}
static void h2o_modify(struct entity* act, u8* buf)
{
}
static void h2o_delete(struct entity* act, u8* buf)
{
}
static void h2o_create(struct entity* act, u8* buf)
{
}




static void h2o_draw_gl41(
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	int j;
	vec3 tc,tr,tf,tt;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vt = geom->f.vt;
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
	struct entity* act, struct style* pin,
	struct entity* wnd, struct style* sty)
{
}
static void h2o_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void h2o_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void h2o_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void h2o_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void h2o_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	if(stack&&('v' == key)){
		slot = stack[sp-1].pfoot;
		wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		h2o_draw_gl41(ent,slot, wor,geom, wnd,area);
	}
}
static void h2o_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void h2o_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void h2o_linkup(struct halfrel* self, struct halfrel* peer)
{
}




void h2o_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex32('h','2','o', 0);

	p->oncreate = (void*)h2o_create;
	p->ondelete = (void*)h2o_delete;
	p->onsearch = (void*)h2o_search;
	p->onmodify = (void*)h2o_modify;

	p->onlinkup = (void*)h2o_linkup;
	p->ondiscon = (void*)h2o_discon;
	p->ontaking = (void*)h2o_taking;
	p->ongiving = (void*)h2o_giving;
}
