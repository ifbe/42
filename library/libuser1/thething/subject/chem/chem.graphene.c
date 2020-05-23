#include "libuser.h"




static void graphene_search(struct entity* act, u8* buf)
{
}
static void graphene_modify(struct entity* act, u8* buf)
{
}
static void graphene_delete(struct entity* act, u8* buf)
{
}
static void graphene_create(struct entity* act, u8* buf)
{
}




static void graphene_draw_gl41(
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

	for(j=0;j<3;j++){
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
		tt[j] = vt[j]/4;
	}
	gl41solid_sphere(ctx, 0x808080, vc, tr, tf, tt);
}
static void graphene_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* wnd, struct style* sty)
{
}
static void graphene_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void graphene_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void graphene_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void graphene_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void graphene_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	if(stack&&('v' == key)){
		slot = stack[sp-1].pfoot;
		wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		graphene_draw_gl41(ent,slot, wor,geom, wnd,area);
	}
}
static void graphene_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void graphene_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void graphene_linkup(struct halfrel* self, struct halfrel* peer)
{
}




void graphene_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('g','r','a','p','h','e','n','e');

	p->oncreate = (void*)graphene_create;
	p->ondelete = (void*)graphene_delete;
	p->onsearch = (void*)graphene_search;
	p->onmodify = (void*)graphene_modify;

	p->onlinkup = (void*)graphene_linkup;
	p->ondiscon = (void*)graphene_discon;
	p->ontaking = (void*)graphene_taking;
	p->ongiving = (void*)graphene_giving;
}
