#include "libuser.h"




static void mux_draw_pixel(
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
static void mux_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	vec3 tc,tr,tf,tu;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vu = geom->f.vt;
	gl41line_prism4(ctx, 0xffffff, vc, vr, vf, vu);
}
static void mux_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void mux_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void mux_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void mux_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void mux_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	if(stack&&('v' == key)){
		slot = stack[sp-1].pfoot;
		wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		mux_draw_gl41(ent,slot, wor,geom, wnd,area);
	}
}
static void mux_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void mux_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void mux_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void mux_search(struct entity* act, u8* buf)
{
}
static void mux_modify(struct entity* act, u8* buf)
{
}
static void mux_delete(struct entity* act, u8* buf)
{
}
static void mux_create(struct entity* act, u8* buf)
{
}




void mux_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex32('m','u','x', 0);

	p->oncreate = (void*)mux_create;
	p->ondelete = (void*)mux_delete;
	p->onsearch = (void*)mux_search;
	p->onmodify = (void*)mux_modify;

	p->onlinkup = (void*)mux_linkup;
	p->ondiscon = (void*)mux_discon;
	p->ontaking = (void*)mux_taking;
	p->ongiving = (void*)mux_giving;
}
