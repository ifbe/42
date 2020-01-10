#include "libuser.h"




static void mosfet_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void mosfet_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void mosfet_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void mosfet_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void mosfet_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void mosfet_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* scn, struct style* geom,
	struct entity* wnd, struct style* area)
{
	int j;
	vec3 tc,tr,tf,tu;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vt = geom->f.vt;
	gl41line_rect(wnd, 0xffffff, vc,vr,vf);
}
static void mosfet_read_bycam(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	struct entity* win;struct style* geom;
	struct entity* act;struct style* slot;
	win = peer->pchip;geom = peer->pfoot;
	act = self->pchip;slot = self->pfoot;
	if(stack && ('v' == len)){
		struct entity* wnd;struct style* area;
		struct entity* wrd;struct style* camg;
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;

		mosfet_draw_gl41(act,slot, wrd,geom, wnd,area);
	}
}
static void mosfet_read_bywnd(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
}




static void mosfet_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	struct supply* sup = peer->pchip;
	switch(sup->fmt){
	case _gl41wnd0_:mosfet_read_bywnd(self,peer, arg,idx, buf,len);break;
	default:	mosfet_read_bycam(self,peer, arg,idx, buf,len);break;
	}
}
static void mosfet_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
}
static void mosfet_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void mosfet_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void mosfet_search(struct entity* act, u8* buf)
{
}
static void mosfet_modify(struct entity* act, u8* buf)
{
}
static void mosfet_delete(struct entity* act, u8* buf)
{
}
static void mosfet_create(struct entity* act, void* arg, int argc, u8** argv)
{
	act->ix0 = 0;	//D
	act->iy0 = 0;	//S
	act->iz0 = 0;	//G
	act->iw0 = 0;	//B
}




void mosfet_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('m','o','s','f','e','t', 0, 0);

	p->oncreate = (void*)mosfet_create;
	p->ondelete = (void*)mosfet_delete;
	p->onsearch = (void*)mosfet_search;
	p->onmodify = (void*)mosfet_modify;

	p->onlinkup = (void*)mosfet_linkup;
	p->ondiscon = (void*)mosfet_discon;
	p->onread  = (void*)mosfet_read;
	p->onwrite = (void*)mosfet_write;
}
