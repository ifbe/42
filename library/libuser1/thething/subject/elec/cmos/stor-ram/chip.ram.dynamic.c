#include "libuser.h"




static void dram_draw_pixel(
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
static void dram_draw_gl41(
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
static void dram_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void dram_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void dram_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void dram_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void dram_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	if(stack&&('v' == key)){
		slot = stack[sp-1].pfoot;
		wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		dram_draw_gl41(ent,slot, wor,geom, wnd,area);
	}
}
static void dram_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void dram_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void dram_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void dram_search(struct entity* act, u8* buf)
{
}
static void dram_modify(struct entity* act, u8* buf)
{
}
static void dram_delete(struct entity* act, u8* buf)
{
}
static void dram_create(struct entity* act, u8* buf)
{
}




void dram_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex32('d','r','a','m');

	p->oncreate = (void*)dram_create;
	p->ondelete = (void*)dram_delete;
	p->onsearch = (void*)dram_search;
	p->onmodify = (void*)dram_modify;

	p->onlinkup = (void*)dram_linkup;
	p->ondiscon = (void*)dram_discon;
	p->ontaking = (void*)dram_taking;
	p->ongiving = (void*)dram_giving;
}
