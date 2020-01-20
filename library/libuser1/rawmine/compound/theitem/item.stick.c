#include "libuser.h"
struct joint{
	float x;
	float y;
	float z;
	int exist;

	float gradx;
	float grady;
	float gradz;
	int sure;
};




static void stick_draw_pixel(
	struct entity* ent, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void stick_draw_json(
	struct entity* ent, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void stick_draw_html(
	struct entity* ent, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void stick_draw_tui(
	struct entity* ent, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void stick_draw_cli(
	struct entity* ent, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void stick_draw_gl41(
	struct entity* ent, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vu = geom->f.vt;
}
static void stick_read_bycam(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	//wnd -> cam, cam -> world
	struct entity* wnd;struct style* area;
	struct entity* wrd;struct style* camg;

	//scene -> terminal
	struct entity* scn;struct style* geom;
	struct entity* ent;struct style* slot;

	if(stack && ('v' == len)){
		ent = self->pchip;slot = self->pfoot;
		scn = peer->pchip;geom = peer->pfoot;
		wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		stick_draw_gl41(ent,slot, scn,geom, wnd,area);
	}
}




static void stick_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	stick_read_bycam(self,peer, arg,idx, buf,len);
}
static void stick_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void stick_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void stick_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void stick_search(struct entity* ent)
{
}
static void stick_modify(struct entity* ent)
{
}
static void stick_delete(struct entity* ent)
{
	if(0 == ent)return;
}
static void stick_create(struct entity* ent, void* arg, int argc, u8** argv)
{
	if(0 == ent)return;
}




void stick_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('s', 'p', 'r', 'i', 'n','g', 0, 0);

	p->oncreate = (void*)stick_create;
	p->ondelete = (void*)stick_delete;
	p->onsearch = (void*)stick_search;
	p->onmodify = (void*)stick_modify;

	p->onlinkup = (void*)stick_linkup;
	p->ondiscon = (void*)stick_discon;
	p->onread  = (void*)stick_read;
	p->onwrite = (void*)stick_write;
}
