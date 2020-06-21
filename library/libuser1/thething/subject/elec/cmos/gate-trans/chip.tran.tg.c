#include "libuser.h"




static void tg_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	int cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->fs.vc[0];
		cy = sty->fs.vc[1];
		ww = sty->fs.vr[0];
		hh = sty->fs.vf[1];
	}
	else
	{
		cx = win->width/2;
		cy = win->height/2;
		ww = win->width/2;
		hh = win->height/2;
	}
}
static void tg_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	vec3 tc,tr,tf,tu;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;
	gl41line_prism4(ctx, 0xffffff, vc, vr, vf, vu);
}
static void tg_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void tg_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void tg_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void tg_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void tg_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	if(stack&&('v' == key)){
		slot = stack[sp-1].pfoot;
		wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		tg_draw_gl41(ent,slot, wor,geom, wnd,area);
	}
}
static void tg_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void tg_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void tg_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void tg_search(struct entity* act, u8* buf)
{
}
static void tg_modify(struct entity* act, u8* buf)
{
}
static void tg_delete(struct entity* act, u8* buf)
{
}
static void tg_create(struct entity* act, u8* buf)
{
}




void tg_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex32('t','g', 0, 0);

	p->oncreate = (void*)tg_create;
	p->ondelete = (void*)tg_delete;
	p->onsearch = (void*)tg_search;
	p->onmodify = (void*)tg_modify;

	p->onlinkup = (void*)tg_linkup;
	p->ondiscon = (void*)tg_discon;
	p->ontaking = (void*)tg_taking;
	p->ongiving = (void*)tg_giving;
}
