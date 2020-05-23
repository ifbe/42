#include "libuser.h"




static void or_draw_pixel(
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
static void or_draw_gl41(
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
static void or_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void or_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void or_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void or_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void or_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	if(stack&&('v' == key)){
		slot = stack[sp-1].pfoot;
		wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		or_draw_gl41(ent,slot, wor,geom, wnd,area);
	}
}
static void or_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void or_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void or_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void or_search(struct entity* act, u8* buf)
{
}
static void or_modify(struct entity* act, u8* buf)
{
}
static void or_delete(struct entity* act, u8* buf)
{
}
static void or_create(struct entity* act, u8* buf)
{
}




void or_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex32('o','r', 0, 0);

	p->oncreate = (void*)or_create;
	p->ondelete = (void*)or_delete;
	p->onsearch = (void*)or_search;
	p->onmodify = (void*)or_modify;

	p->onlinkup = (void*)or_linkup;
	p->ondiscon = (void*)or_discon;
	p->ontaking = (void*)or_taking;
	p->ongiving = (void*)or_giving;
}
