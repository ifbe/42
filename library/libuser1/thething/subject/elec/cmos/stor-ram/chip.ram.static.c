#include "libuser.h"




static void sram_draw_pixel(
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
static void sram_draw_gl41(
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
static void sram_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void sram_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void sram_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void sram_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void sram_wrl_cam_wnd(_ent* ent,void* slot, _syn* stack,int sp)
{
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	sram_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void sram_taking(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	//foot defined behavior
	switch(stack[sp-1].flag){
	}

	//caller defined behavior
	struct entity* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(caller->fmt){
	case _rgba_:
		break;
	case _gl41full_:
		break;
	default:
		sram_wrl_cam_wnd(ent,foot, stack,sp);
		break;
	}
}
static void sram_giving(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void sram_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void sram_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void sram_search(struct entity* act, u8* buf)
{
}
static void sram_modify(struct entity* act, u8* buf)
{
}
static void sram_delete(struct entity* act, u8* buf)
{
}
static void sram_create(struct entity* act, u8* buf)
{
}




void sram_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex32('s','r','a','m');

	p->oncreate = (void*)sram_create;
	p->ondelete = (void*)sram_delete;
	p->onsearch = (void*)sram_search;
	p->onmodify = (void*)sram_modify;

	p->onlinkup = (void*)sram_linkup;
	p->ondiscon = (void*)sram_discon;
	p->ontaking = (void*)sram_taking;
	p->ongiving = (void*)sram_giving;
}
