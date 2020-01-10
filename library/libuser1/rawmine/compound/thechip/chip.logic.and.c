#include "libuser.h"




static void and_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	int cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->f.vc[0];
		cy = sty->f.vc[1];
		ww = sty->f.vr[0];
		hh = sty->f.vf[1];
	}
	else
	{
		cx = win->width/2;
		cy = win->height/2;
		ww = win->width/2;
		hh = win->height/2;
	}
}
static void and_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	vec3 tc,tr,tf,tu;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vu = geom->f.vt;
	carveline_prism4(ctx, 0xffffff, vc, vr, vf, vu);
}
static void and_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void and_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void and_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void and_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void and_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, u8* buf, int len)
{
//wnd -> cam, cam -> world
	struct entity* wnd;struct style* area;
	struct entity* wrd;struct style* camg;
//world -> and
	struct entity* win;struct style* geom;
	struct entity* act;struct style* slot;

	if(stack){
		act = self->pchip;slot = self->pfoot;
		win = peer->pchip;geom = peer->pfoot;
		wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		if('v' == len)and_draw_gl41(act,slot, win,geom, wnd,area);
	}
}
static void and_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
}
static void and_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void and_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void and_search(struct entity* act, u8* buf)
{
}
static void and_modify(struct entity* act, u8* buf)
{
}
static void and_delete(struct entity* act, u8* buf)
{
}
static void and_create(struct entity* act, u8* buf)
{
}




void and_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex32('a','n','d', 0);

	p->oncreate = (void*)and_create;
	p->ondelete = (void*)and_delete;
	p->onsearch = (void*)and_search;
	p->onmodify = (void*)and_modify;

	p->onlinkup = (void*)and_linkup;
	p->ondiscon = (void*)and_discon;
	p->onread  = (void*)and_read;
	p->onwrite = (void*)and_write;
}
