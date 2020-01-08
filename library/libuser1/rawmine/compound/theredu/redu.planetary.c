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




static void planetary_draw_vbo(
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	int j;
	float r,a,c,s;
	vec3 tc,tr,tf,tt;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vt = geom->f.vt;

	r = tau*(timeread()%10000000)/10000000.0;
	carveline_rotategear(ctx, 0x444444, vc, vr, vf, 48, r*18/48.0);

	for(j=0;j<3;j++){
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	carvesolid_rotategear(ctx, 0x808080, vc, tr, tf, vt, 12, -r*18/12.0+tau/24);

	//planet 0
	for(j=0;j<3;j++){
		tc[j] = vc[j] + vr[j]*0.575;
		tr[j] = vr[j]*0.35;
		tf[j] = vf[j]*0.35;
	}
	carvesolid_rotategear(ctx, 0x808080, tc, tr, tf, vt, 18, r);

	//planet 1
	a = 120*PI/180;
	c = cosine(a)*0.575;
	s = sine(a)*0.575;
	for(j=0;j<3;j++)tc[j] = vc[j] + vr[j]*c + vf[j]*s;
	carvesolid_rotategear(ctx, 0x808080, tc, tr, tf, vt, 18, r);

	//planet 2
	a = 240*PI/180;
	c = cosine(a)*0.575;
	s = sine(a)*0.575;
	for(j=0;j<3;j++)tc[j] = vc[j] + vr[j]*c + vf[j]*s;
	carvesolid_rotategear(ctx, 0x808080, tc, tr, tf, vt, 18, r);
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




static void planetary_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
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
		if('v' == len)planetary_draw_vbo(act,slot, win,geom, wnd,area);
	}
}
static void planetary_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void planetary_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void planetary_start(struct halfrel* self, struct halfrel* peer)
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

	p->onstart = (void*)planetary_start;
	p->onstop  = (void*)planetary_stop;
	p->onread  = (void*)planetary_read;
	p->onwrite = (void*)planetary_write;
}
