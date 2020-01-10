#include "libuser.h"




static void resistor_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void resistor_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* scn, struct style* geom,
	struct entity* wnd, struct style* area)
{
	int j;
	vec3 tc,tr,tf,tt;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vt = geom->f.vt;

	for(j=0;j<3;j++)tc[j] = vc[j];
	carvesolid_prism4(wnd, 0x808080, vc, vr, vf, vt);
}
static void resistor_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void resistor_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void resistor_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void resistor_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void resistor_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
//wnd -> cam, cam -> world
	struct entity* wnd;struct style* area;
	struct entity* wrd;struct style* camg;
//world -> resistor
	struct entity* win;struct style* geom;
	struct entity* act;struct style* slot;

	if(stack){
		act = self->pchip;slot = self->pfoot;
		win = peer->pchip;geom = peer->pfoot;
		wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		if('v' == len){
			resistor_draw_gl41(act,slot, win,geom, wnd,area);
		}
	}
}
static void resistor_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void resistor_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void resistor_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void resistor_search(struct entity* act, u8* buf)
{
}
static void resistor_modify(struct entity* act, u8* buf)
{
}
static void resistor_delete(struct entity* act, u8* buf)
{
}
static void resistor_create(struct entity* act, u8* buf)
{
}




void resistor_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('r','e','s','i','s','t','o','r');

	p->oncreate = (void*)resistor_create;
	p->ondelete = (void*)resistor_delete;
	p->onsearch = (void*)resistor_search;
	p->onmodify = (void*)resistor_modify;

	p->onlinkup = (void*)resistor_linkup;
	p->ondiscon = (void*)resistor_discon;
	p->onread  = (void*)resistor_read;
	p->onwrite = (void*)resistor_write;
}
