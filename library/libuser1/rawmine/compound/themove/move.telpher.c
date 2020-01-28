#include "libuser.h"




static void telpher_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void telpher_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void telpher_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void telpher_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void telpher_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void telpher_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* scn, struct style* geom,
	struct entity* wnd, struct style* area)
{
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vt = geom->f.vt;
	gl41line_prism4(wnd, 0xffffff, vc, vr, vf, vt);
}
static void telpher_read_bycam(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	//wnd -> cam, cam -> world
	struct entity* wnd;struct style* area;
	struct entity* wrd;struct style* camg;

	//scene -> clock
	struct entity* scn;struct style* geom;
	struct entity* act;struct style* slot;

	if(stack && ('v' == len)){
		act = self->pchip;slot = self->pfoot;
		scn = peer->pchip;geom = peer->pfoot;
		wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		telpher_draw_gl41(act,slot, scn,geom, wnd,area);
	}
}




static void telpher_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	telpher_read_bycam(self,peer, stack,rsp, buf,len);
}
static void telpher_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void telpher_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void telpher_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void telpher_search(struct entity* act)
{
}
static void telpher_modify(struct entity* act)
{
}
static void telpher_delete(struct entity* act)
{
}
static void telpher_create(struct entity* act)
{
}




void telpher_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('t','e','l','p','h','e','r', 0);

	p->oncreate = (void*)telpher_create;
	p->ondelete = (void*)telpher_delete;
	p->onsearch = (void*)telpher_search;
	p->onmodify = (void*)telpher_modify;

	p->onlinkup = (void*)telpher_linkup;
	p->ondiscon = (void*)telpher_discon;
	p->onread  = (void*)telpher_read;
	p->onwrite = (void*)telpher_write;
}
