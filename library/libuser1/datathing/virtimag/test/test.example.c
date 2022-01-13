#include "libuser.h"
u32 getrandom();




static void example_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
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
		cx = win->whdf.width/2;
		cy = win->whdf.height/2;
		ww = win->whdf.width/2;
		hh = win->whdf.height/2;
	}

	bg = getrandom()&0xffffff;
	fg = (~bg)&0xffffff;
	drawsolid_rect(win, bg, cx-ww, cy-hh, cx+ww, cy+hh);
	drawhexadecimal(win, fg, cx, cy, bg);
}
static void example_draw_gl41(
	_obj* act, struct style* slot,
	_obj* wrl, struct style* geom,
	_obj* wnd, struct style* area)
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
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void example_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void example_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void example_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void example_wrl_cam_wnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	example_draw_gl41(ent,slot, wor,geom, wnd,area);
}
static void example_wrl_wnd(_obj* ent,void* slot, _syn* stack,int sp)
{
}
static void example_wnd(_obj* ent,void* slot, _syn* stack,int sp)
{
}




static void example_taking(_obj* ent,void* slot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	//foot defined behavior
	switch(stack[sp-1].foottype){
	}

	//caller defined behavior
	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(caller->hfmt){
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
static void example_giving(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void example_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void example_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void example_search(_obj* act)
{
}
static void example_modify(_obj* act)
{
}
static void example_delete(_obj* act)
{
	if(0 == act)return;
}
static void example_create(_obj* act)
{
	if(0 == act)return;
}




void example_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('e', 'x', 'a', 'm', 'p', 'l', 'e', 0);

	p->oncreate = (void*)example_create;
	p->ondelete = (void*)example_delete;
	p->onsearch = (void*)example_search;
	p->onmodify = (void*)example_modify;

	p->onattach = (void*)example_attach;
	p->ondetach = (void*)example_detach;
	p->ontaking = (void*)example_taking;
	p->ongiving = (void*)example_giving;
}
