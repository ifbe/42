#include "libuser.h"




static void tff_draw_pixel(
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
static void tff_draw_gl41(
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
static void tff_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void tff_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void tff_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void tff_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void tff_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	if(stack&&('v' == key)){
		slot = stack[sp-1].pfoot;
		wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		tff_draw_gl41(ent,slot, wor,geom, wnd,area);
	}
}
static void tff_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void tff_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void tff_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void tff_search(struct entity* act, u8* buf)
{
}
static void tff_modify(struct entity* act, u8* buf)
{
}
static void tff_delete(struct entity* act, u8* buf)
{
}
static void tff_create(struct entity* act, u8* buf)
{
}




void tff_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex32('t','f','f', 0);

	p->oncreate = (void*)tff_create;
	p->ondelete = (void*)tff_delete;
	p->onsearch = (void*)tff_search;
	p->onmodify = (void*)tff_modify;

	p->onlinkup = (void*)tff_linkup;
	p->ondiscon = (void*)tff_discon;
	p->ontaking = (void*)tff_taking;
	p->ongiving = (void*)tff_giving;
}
