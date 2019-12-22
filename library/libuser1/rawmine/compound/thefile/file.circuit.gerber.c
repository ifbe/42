#include "libuser.h"




static void rs274x_draw_vbo3d(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vu = geom->f.vt;
	carveline_rect(ctx, 0x404040, vc, vr, vf);
}




static void rs274x_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
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
		if('v' == len)rs274x_draw_vbo3d(act,slot, scn,geom, wnd,area);
	}
}
static void rs274x_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void rs274x_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void rs274x_start(struct halfrel* self, struct halfrel* peer)
{
}




static void rs274x_search(struct entity* act)
{
}
static void rs274x_modify(struct entity* act)
{
}
static void rs274x_delete(struct entity* act)
{
	if(0 == act)return;
	//if(_copy_ == act->type)memorydelete(act->buf);
}
static void rs274x_create(struct entity* act)
{
	if(0 == act)return;
	//if(_orig_ == act->type)act->buf = buffer;
	//if(_copy_ == act->type)act->buf = memorycreate(256, 0);
}




void rs274x_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('r', 's', '2', '7', '4','x', 0, 0);

	p->oncreate = (void*)rs274x_create;
	p->ondelete = (void*)rs274x_delete;
	p->onsearch = (void*)rs274x_search;
	p->onmodify = (void*)rs274x_modify;

	p->onstart = (void*)rs274x_start;
	p->onstop  = (void*)rs274x_stop;
	p->onread  = (void*)rs274x_read;
	p->onwrite = (void*)rs274x_write;
}
