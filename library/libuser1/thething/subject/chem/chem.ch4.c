#include "libuser.h"




static void ch4_search(struct entity* act, u8* buf)
{
}
static void ch4_modify(struct entity* act, u8* buf)
{
}
static void ch4_delete(struct entity* act, u8* buf)
{
}
static void ch4_create(struct entity* act, u8* buf)
{
}




static void ch4_draw_gl41(
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	int j;
	vec3 tc,tr,tf,tt;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
	gl41line_prism4(ctx, 0x444444, vc, vr, vf, vt);

	//carbon
	for(j=0;j<3;j++){
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
		tt[j] = vt[j]/4;
	}
	gl41solid_sphere(ctx, 0x808080, vc, tr, tf, tt);

	for(j=0;j<3;j++){
		tr[j] = vr[j]/8;
		tf[j] = vf[j]/8;
		tt[j] = vt[j]/8;
		tc[j] = vc[j] +vt[j]-tt[j];
	}
	gl41solid_sphere(ctx, 0x808080, tc, tr, tf, tt);
}
static void ch4_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* wnd, struct style* sty)
{
}
static void ch4_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void ch4_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void ch4_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void ch4_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void ch4_taking(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	if(stack&&('v' == key)){
		slot = stack[sp-1].pfoot;
		wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		ch4_draw_gl41(ent,slot, wor,geom, wnd,area);
	}
}
static void ch4_giving(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void ch4_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void ch4_linkup(struct halfrel* self, struct halfrel* peer)
{
}




void ch4_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex32('c','h','4', 0);

	p->oncreate = (void*)ch4_create;
	p->ondelete = (void*)ch4_delete;
	p->onsearch = (void*)ch4_search;
	p->onmodify = (void*)ch4_modify;

	p->onlinkup = (void*)ch4_linkup;
	p->ondiscon = (void*)ch4_discon;
	p->ontaking = (void*)ch4_taking;
	p->ongiving = (void*)ch4_giving;
}
