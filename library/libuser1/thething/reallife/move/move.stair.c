#include "libuser.h"




static void stair_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void stair_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void stair_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void stair_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void stair_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void stair_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* scn, struct style* geom,
	struct entity* wnd, struct style* area)
{
	int z,j;
	vec3 tc,tr,tf,tt;
	vec3 kc,kr,kf,kt;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
	//gl41line_prism4(wnd, 0xffffff, vc, vr, vf, vt);

	for(j=0;j<3;j++){
		tr[j] = vr[j]/8;
		tf[j] = vf[j]/8;
		tt[j] = vt[j]/64;
	}
	for(z=0;z<8;z++)
	{
		for(j=0;j<3;j++)tc[j] = vc[j] + tf[j]*(z*2-7) + vt[j]*(z+1)/9-tt[j];
		gl41solid_prism4(wnd, 0x808080, tc, vr, tf, tt);
	}

	for(j=0;j<3;j++){
		kr[j] = vr[j]/64;
		kf[j] = vf[j]/64;
		kt[j] = vt[j]/18;
	}
	for(z=0;z<=8;z++)
	{
		for(j=0;j<3;j++)kc[j] = vc[j] -vr[j]+kr[j] + tf[j]*(z*2-8) + vt[j]*(z+1)/9-kt[j];
		gl41solid_cylinder(wnd, 0x800000, kc, kr, kf, kt);

		for(j=0;j<3;j++)kc[j] = vc[j] +vr[j]-kr[j] + tf[j]*(z*2-8) + vt[j]*(z+1)/9-kt[j];
		gl41solid_cylinder(wnd, 0x800000, kc, kr, kf, kt);
	}
}
static void stair_read_bycam(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct style* slot;
	struct entity* scn;struct style* geom;
	struct entity* wnd;struct style* area;

	if(stack && ('v'==key)){
		slot = stack[sp-1].pfoot;
		scn = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		stair_draw_gl41(ent,slot, scn,geom, wnd,area);
	}
}




static void stair_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	stair_read_bycam(ent,foot, stack,sp, arg,key, buf,len);
}
static void stair_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void stair_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void stair_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void stair_search(struct entity* act)
{
}
static void stair_modify(struct entity* act)
{
}
static void stair_delete(struct entity* act)
{
	if(0 == act)return;
	//if(_copy_ == act->type)memorydelete(act->buf);
}
static void stair_create(struct entity* act)
{
	if(0 == act)return;
	//if(_orig_ == act->type)act->buf = buffer;
	//if(_copy_ == act->type)act->buf = memorycreate(256, 0);
}




void stair_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('s', 't', 'a', 'i', 'r', 0, 0, 0);

	p->oncreate = (void*)stair_create;
	p->ondelete = (void*)stair_delete;
	p->onsearch = (void*)stair_search;
	p->onmodify = (void*)stair_modify;

	p->onlinkup = (void*)stair_linkup;
	p->ondiscon = (void*)stair_discon;
	p->ontaking = (void*)stair_taking;
	p->ongiving = (void*)stair_giving;
}
