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
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vt = geom->f.vt;
	carveline_prism4(ctx, 0x444444, vc, vr, vf, vt);

	//carbon
	for(j=0;j<3;j++){
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
		tt[j] = vt[j]/4;
	}
	carvesolid_sphere(ctx, 0x808080, vc, tr, tf, tt);

	for(j=0;j<3;j++){
		tr[j] = vr[j]/8;
		tf[j] = vf[j]/8;
		tt[j] = vt[j]/8;
		tc[j] = vc[j] +vt[j]-tt[j];
	}
	carvesolid_sphere(ctx, 0x808080, tc, tr, tf, tt);
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




static void ch4_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
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
		if('v' == len)ch4_draw_gl41(act,slot, win,geom, wnd,area);
	}
}
static void ch4_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
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
	p->onread  = (void*)ch4_read;
	p->onwrite = (void*)ch4_write;
}
