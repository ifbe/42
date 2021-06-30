#include "libuser.h"
u32 getrandom();




static void example_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	u32 bg,fg;
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

	bg = getrandom()&0xffffff;
	fg = (~bg)&0xffffff;
	drawsolid_rect(win, bg, cx-ww, cy-hh, cx+ww, cy+hh);
	drawhexadecimal(win, fg, cx, cy, bg);
}
static void example_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* wrl, struct style* geom,
	struct entity* wnd, struct style* area)
{
	vec3 tc, tr, tf, tu, f;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;

	tr[0] = vr[0]/2;
	tr[1] = vr[1]/2;
	tr[2] = vr[2]/2;
	tf[0] = vf[0]/2;
	tf[1] = vf[1]/2;
	tf[2] = vf[2]/2;
	tu[0] = vu[0]/2;
	tu[1] = vu[1]/2;
	tu[2] = vu[2]/2;

	tc[0] = vc[0]-vr[0]/2-vf[0]/2+vu[0]/2;
	tc[1] = vc[1]-vr[1]/2-vf[1]/2+vu[1]/2;
	tc[2] = vc[2]-vr[2]/2-vf[2]/2+vu[2]/2,
	gl41point_sphere(wnd, 0xffffff, tc, tr, tf, tu);

	tc[0] = vc[0]+vr[0]/2-vf[0]/2+vu[0]/2;
	tc[1] = vc[1]+vr[1]/2-vf[1]/2+vu[1]/2;
	tc[2] = vc[2]+vr[2]/2-vf[2]/2+vu[2]/2;
	gl41line_sphere(wnd, 0xffffff, tc, tr, tf, tu);

	tc[0] = vc[0]-vr[0]/2+vf[0]/2+vu[0]/2;
	tc[1] = vc[1]-vr[1]/2+vf[1]/2+vu[1]/2;
	tc[2] = vc[2]-vr[2]/2+vf[2]/2+vu[2]/2;
	gl41solid_sphere(wnd, 0xff00ff, tc, tr, tf, tu);

	tc[0] = vc[0]-vr[0]/2-vf[0]/2+vu[0]*3/2;
	tc[1] = vc[1]-vr[1]/2-vf[1]/2+vu[1]*3/2;
	tc[2] = vc[2]-vr[2]/2-vf[2]/2+vu[2]*3/2;
	gl41point_sphere(wnd, 0xffffff, tc, tr, tf, tu);

	tc[0] = vc[0]+vr[0]/2-vf[0]/2+vu[0]*3/2;
	tc[1] = vc[1]+vr[1]/2-vf[1]/2+vu[1]*3/2;
	tc[2] = vc[2]+vr[2]/2-vf[2]/2+vu[2]*3/2;
	gl41line_sphere(wnd, 0xffffff, tc, tr, tf, tu);

	tc[0] = vc[0]-vr[0]/2+vf[0]/2+vu[0]*3/2;
	tc[1] = vc[1]-vr[1]/2+vf[1]/2+vu[1]*3/2;
	tc[2] = vc[2]-vr[2]/2+vf[2]/2+vu[2]*3/2;
	gl41solid_sphere(wnd, 0x87cefa, tc, tr, tf, tu);
}
static void example_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void example_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void example_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void example_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void example_wrl_cam_wnd(_ent* ent,void* slot, _syn* stack,int sp)
{
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	example_draw_gl41(ent,slot, wor,geom, wnd,area);
}
static void example_wrl_wnd(_ent* ent,void* slot, _syn* stack,int sp)
{
}
static void example_wnd(_ent* ent,void* slot, _syn* stack,int sp)
{
}




static void example_taking(_ent* ent,void* slot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
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
		example_draw_pixel(ent,slot, caller, area);
		break;
	case _gl41list_:
		example_wnd(ent,slot, stack,sp);
		break;
	case _scene3d_:
		example_wrl_wnd(ent,slot, stack,sp);
		break;
	default:
		example_wrl_cam_wnd(ent,slot, stack,sp);
		break;
	}
}
static void example_giving(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void example_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void example_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void example_search(struct entity* act)
{
}
static void example_modify(struct entity* act)
{
}
static void example_delete(struct entity* act)
{
	if(0 == act)return;
}
static void example_create(struct entity* act)
{
	if(0 == act)return;
}




void example_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('e', 'x', 'a', 'm', 'p', 'l', 'e', 0);

	p->oncreate = (void*)example_create;
	p->ondelete = (void*)example_delete;
	p->onsearch = (void*)example_search;
	p->onmodify = (void*)example_modify;

	p->onlinkup = (void*)example_linkup;
	p->ondiscon = (void*)example_discon;
	p->ontaking = (void*)example_taking;
	p->ongiving = (void*)example_giving;
}
