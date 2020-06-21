#include "libuser.h"




static void cpure_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;
	gl41line_rect(ctx, 0x404040, vc, vr, vf);
}




static void cpure_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct style* slot;
	struct entity* scn;struct style* geom;
	struct entity* wnd;struct style* area;

	if(stack&&('v'==key)){
		slot = stack[sp-1].pfoot;
		scn = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		cpure_draw_gl41(ent,slot, scn,geom, wnd,area);
	}
}
static void cpure_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
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
	p->ontaking = (void*)cpure_taking;
	p->ongiving = (void*)cpure_giving;
}
