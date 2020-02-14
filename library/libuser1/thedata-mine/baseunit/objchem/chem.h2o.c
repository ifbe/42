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




static void h2o_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	//world -> 2048
	struct entity* win;struct style* geom;
	struct entity* act;struct style* slot;
	act = self->pchip;slot = self->pfoot;
	win = peer->pchip;geom = peer->pfoot;
	if(stack){
	//wnd -> cam, cam -> world
		struct entity* wnd;struct style* area;
		struct entity* wrd;struct style* camg;
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;
		if('v' == len)h2o_draw_gl41(act,slot, win,geom, wnd,area);
	}
}
static void h2o_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
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
	p->onread  = (void*)h2o_read;
	p->onwrite = (void*)h2o_write;
}
