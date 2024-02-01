#include "libuser.h"




static void wrl3d_gl41draw(
	_obj* act, struct style* part,
	_obj* win, struct style* geom,
	_obj* wrd, struct style* camg,
	_obj* wnd, struct style* area)
{
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;
	gl41solid_prism4(wnd, 0x444444, vc, vr, vf, vu);
}




static void wrl3d_tui_test(
	_obj* act, struct style* pin,
	_obj* wnd, struct style* sty)
{
}
static void wrl3d_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void wrl3d_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void wrl3d_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	logtoall("wrl3d(%x,%x,%x)\n",win,act,sty);
}




//[-6,-5]: wnd -> cam
//[-4,-3]: cam -> world
//[-2,-1]: world -> wrl3d
static void wrl3d_world_camera_window(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* scn;struct style* geom;
	_obj* wrd;struct style* camg;
	_obj* wnd;struct style* area;

	scn = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wrd = stack[sp-3].pchip;camg = stack[sp-3].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;

	switch(wnd->type){
	case _gl41list_:
		wrl3d_gl41draw(ent,slot, scn,geom, wrd,camg, wnd,area);
		break;
	}
}
static void wrl3d_taking(_obj* ent,void* slot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	//foot defined behavior
	switch(stack[sp-1].foottype){
	}

	//caller defined behavior
	switch(caller->type){
	case _rgba_:
	        break;
	case _gl41list_:
	        break;
	default:
		wrl3d_world_camera_window(ent,slot, stack,sp);
	}
}
static void wrl3d_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void wrl3d_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void wrl3d_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void wrl3d_search(_obj* act)
{
}
static void wrl3d_modify(_obj* act)
{
}
static void wrl3d_delete(_obj* act)
{
}
static void wrl3d_create(_obj* act, void* str, int argc, u8** argv)
{
}




void wrl3d_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex64('w','r','l','3','d', 0, 0, 0);

	p->oncreate = (void*)wrl3d_create;
	p->ondelete = (void*)wrl3d_delete;
	p->onreader = (void*)wrl3d_search;
	p->onwriter = (void*)wrl3d_modify;

	p->onattach = (void*)wrl3d_attach;
	p->ondetach = (void*)wrl3d_detach;
	p->ontaking = (void*)wrl3d_taking;
	p->ongiving = (void*)wrl3d_giving;
}
