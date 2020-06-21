#include "libuser.h"




static void flower_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void flower_draw_gl41(
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	int j;
	float len;
	vec3 tc,tr,tf,tt;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;

	for(j=0;j<3;j++){
		tc[j] = vc[j] + vt[j]*9/20;
		tr[j] = vr[j]/8;
		tf[j] = vf[j]/8;
		tt[j] = vt[j]*9/20;
	}
	gl41solid_cylinder(ctx, 0x00ff00, tc,tr,tf,tt);

	len = vec3_getlen(tr);
	//vec3_setlen(tr, len);
	vec3_setlen(tf, len);
	vec3_setlen(tt, len);
	for(j=0;j<3;j++){
		tc[j] = vc[j] + vt[j]*19/20;
	}
	gl41solid_sphere(ctx, 0xffff00, tc,tr,tf,tt);

	for(j=0;j<3;j++){
		tc[j] = vc[j] + vr[j]/2 + vt[j]*19/20;
		tr[j] = vr[j]/2 + vt[j]/20;
		tf[j] = vf[j]/2;
	}
	for(j=0;j<6;j++){
		gl41solid_circle(ctx, 0xff0000, tc,tr,tf);
		quaternion_operation(tc, vt, PI/3);
		quaternion_operation(tr, vt, PI/3);
		quaternion_operation(tf, vt, PI/3);
	}
}
static void flower_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void flower_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void flower_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void flower_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void flower_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct style* slot;
	struct entity* scn;struct style* geom;
	struct entity* wnd;struct style* area;

	if(stack && ('v'==key)){
		slot = stack[sp-1].pfoot;
		scn = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		flower_draw_gl41(ent,slot, scn,geom, wnd,area);
	}
}
static void flower_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void flower_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void flower_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void flower_search(struct entity* act)
{
}
static void flower_modify(struct entity* act)
{
}
static void flower_delete(struct entity* act)
{
}
static void flower_create(struct entity* act)
{
}




void flower_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('f','l','o','w','e','r', 0, 0);

	p->oncreate = (void*)flower_create;
	p->ondelete = (void*)flower_delete;
	p->onsearch = (void*)flower_search;
	p->onmodify = (void*)flower_modify;

	p->onlinkup = (void*)flower_linkup;
	p->ondiscon = (void*)flower_discon;
	p->ontaking = (void*)flower_taking;
	p->ongiving = (void*)flower_giving;
}
