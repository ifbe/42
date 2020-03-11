#include "libuser.h"




static void minimap_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	float* vc = geom->fshape.vc;
	float* vr = geom->fshape.vr;
	float* vf = geom->fshape.vf;
	float* vt = geom->fshape.vt;
	gl41solid_rect(ctx,0xff00ff, vc,vr,vf);
}
static void minimap_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void minimap_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void minimap_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void minimap_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void minimap_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void minimap_read_bycam(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
//wnd -> cam, cam -> world
	struct entity* wnd;struct style* area;
	struct entity* wor;struct style* camg;

	//world -> minimap
	struct entity* win;struct style* geom;
	struct entity* act;struct style* slot;

	if(stack&&('v'==len)){
		act = self->pchip;slot = self->pfoot;
		win = peer->pchip;geom = peer->pfoot;
		wor = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		minimap_draw_gl41(act,slot, win,geom, wnd,area);
	}
}
static void minimap_read_byuiux(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	struct entity* wnd;struct style* rect;
	struct entity* uuu;struct style* area;
	struct entity* map;struct style* gl41;
	wnd = stack[rsp-4]->pchip;rect = stack[rsp-4]->pfoot;
	uuu = stack[rsp-2]->pchip;area = stack[rsp-2]->pfoot;
	map = self->pchip;gl41 = self->pfoot;

	//0,1 -> -1,1
	float x0,y0,dx,dy;
	x0 = area->fs.vc[0]*2-1;
	y0 = area->fs.vc[1]*2-1;
	dx = area->fs.vq[0];
	dy = area->fs.vq[1];
	//say("%f,%f,%f,%f\n",x0,y0,dx,dy);

	struct fstyle fs;
	fs.vc[0] = x0+dx;fs.vc[1] = y0+dy;fs.vc[2] = 0.0;
	fs.vr[0] = dx;fs.vr[1] = 0.0;fs.vr[2] = 0.0;
	fs.vf[0] = 0.0;fs.vf[1] = dy;fs.vf[2] = 0.0;
	fs.vt[0] = 0.0;fs.vt[1] = 0.0;fs.vt[2] = 1.0;

	minimap_draw_gl41(map, 0, 0,(void*)&fs, wnd,rect);
}
static void minimap_read_bywnd(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
}




static void minimap_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	struct entity* ent = self->pchip;
	struct supply* sup = peer->pchip;
//say("fmt=%.8s\n", &sup->fmt);
	switch(sup->fmt){
	case _gl41wnd0_:
	case _full_:
	case _wnd_:{
		if('v' != len)break;
		minimap_read_bywnd(self, peer, stack, rsp, buf, len);
		break;
	}
	case _uiuxroot_:{
		minimap_read_byuiux(self, peer, stack, rsp, buf, len);
		break;
	}
	default:{
		minimap_read_bycam(self, peer, stack, rsp, buf, len);
		break;
	}
	}
}
static void minimap_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void minimap_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void minimap_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void minimap_search(struct entity* act)
{
}
static void minimap_modify(struct entity* act)
{
}
static void minimap_delete(struct entity* act)
{
}
static void minimap_create(struct entity* act, u8* arg)
{
}




void minimap_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('m', 'i', 'n', 'i', 'm', 'a', 'p', 0);

	p->oncreate = (void*)minimap_create;
	p->ondelete = (void*)minimap_delete;
	p->onsearch = (void*)minimap_search;
	p->onmodify = (void*)minimap_modify;

	p->onlinkup = (void*)minimap_linkup;
	p->ondiscon = (void*)minimap_discon;
	p->onread  = (void*)minimap_read;
	p->onwrite = (void*)minimap_write;
}
