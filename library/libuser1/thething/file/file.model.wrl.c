#include "libuser.h"




static void wrl3d_gl41draw(
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* wrd, struct style* camg,
	struct entity* wnd, struct style* area)
{
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;
	gl41solid_prism4(wnd, 0x444444, vc, vr, vf, vu);
}




static void wrl3d_tui_test(
	struct entity* act, struct style* pin,
	struct entity* wnd, struct style* sty)
{
}
static void wrl3d_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void wrl3d_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void wrl3d_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	say("wrl3d(%x,%x,%x)\n",win,act,sty);
}




//[-6,-5]: wnd -> cam
//[-4,-3]: cam -> world
//[-2,-1]: world -> wrl3d
static void wrl3d_world_camera_window(_ent* ent,void* slot, _syn* stack,int sp)
{
	struct entity* scn;struct style* geom;
	struct entity* wrd;struct style* camg;
	struct entity* wnd;struct style* area;

	scn = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wrd = stack[sp-3].pchip;camg = stack[sp-3].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;

	switch(wnd->fmt){
	case _gl41full_:
		wrl3d_gl41draw(ent,slot, scn,geom, wrd,camg, wnd,area);
		break;
	}
}
static void wrl3d_taking(_ent* ent,void* slot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	struct entity* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	//foot defined behavior
	switch(stack[sp-1].flag){
	}

	//caller defined behavior
	switch(caller->fmt){
	case _rgba_:
	        break;
	case _gl41full_:
	        break;
	default:
		wrl3d_world_camera_window(ent,slot, stack,sp);
	}
}
static void wrl3d_giving(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void wrl3d_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void wrl3d_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void wrl3d_search(struct entity* act)
{
}
static void wrl3d_modify(struct entity* act)
{
}
static void wrl3d_delete(struct entity* act)
{
}
static void wrl3d_create(struct entity* act, void* str, int argc, u8** argv)
{
}




void wrl3d_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('w','r','l','3','d', 0, 0, 0);

	p->oncreate = (void*)wrl3d_create;
	p->ondelete = (void*)wrl3d_delete;
	p->onsearch = (void*)wrl3d_search;
	p->onmodify = (void*)wrl3d_modify;

	p->onlinkup = (void*)wrl3d_linkup;
	p->ondiscon = (void*)wrl3d_discon;
	p->ontaking = (void*)wrl3d_taking;
	p->ongiving = (void*)wrl3d_giving;
}
