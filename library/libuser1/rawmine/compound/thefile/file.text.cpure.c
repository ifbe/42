#include "libuser.h"




static void cpure_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vu = geom->f.vt;
	gl41line_rect(ctx, 0x404040, vc, vr, vf);
}




static void cpure_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	//wnd -> cam, cam -> world
	struct entity* wnd;struct style* area;
	struct entity* wrd;struct style* camg;

	//scene -> terminal
	struct entity* scn;struct style* geom;
	struct entity* act;struct style* slot;

	if(stack){
		act = self->pchip;slot = self->pfoot;
		scn = peer->pchip;geom = peer->pfoot;
		wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		if('v' == len)cpure_draw_gl41(act,slot, scn,geom, wnd,area);
	}
}
static void cpure_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void cpure_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void cpure_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void cpure_search(struct entity* act)
{
}
static void cpure_modify(struct entity* act)
{
}
static void cpure_delete(struct entity* act)
{
	if(0 == act)return;
	//if(_copy_ == act->type)memorydelete(act->buf);
}
static void cpure_create(struct entity* act)
{
	if(0 == act)return;
	//if(_orig_ == act->type)act->buf = buffer;
	//if(_copy_ == act->type)act->buf = memorycreate(256, 0);
}




void cpure_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('c', 'p', 'u', 'r', 'e', 0, 0, 0);

	p->oncreate = (void*)cpure_create;
	p->ondelete = (void*)cpure_delete;
	p->onsearch = (void*)cpure_search;
	p->onmodify = (void*)cpure_modify;

	p->onlinkup = (void*)cpure_linkup;
	p->ondiscon = (void*)cpure_discon;
	p->onread  = (void*)cpure_read;
	p->onwrite = (void*)cpure_write;
}
