#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151




static void spider_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void spider_draw_gl41(
	struct entity* act, struct style* part,
	struct entity* scn, struct style* geom,
	struct entity* ctx, struct style* area)
{
}
static void spider_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void spider_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void spider_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void spider_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void spider_taking(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	if(stack&&('v' == key)){
		slot = stack[sp-1].pfoot;
		wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		spider_draw_gl41(ent,slot, wor,geom, wnd,area);
	}
}
static void spider_giving(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void spider_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void spider_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void spider_modify(struct entity* act)
{
}
static void spider_search(struct entity* act)
{
}
static void spider_delete(struct entity* act)
{
	if(0 == act)return;
	//if(_copy_ == act->type)memorydelete(act->buf);
}
static void spider_create(struct entity* act)
{
	if(0 == act)return;
	//if(_orig_ == act->type)act->buf = buffer;
	//if(_copy_ == act->type)act->buf = memorycreate(256, 0);
}




void spider_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('s', 'p', 'i', 'd', 'e', 'r', 0, 0);

	p->oncreate = (void*)spider_create;
	p->ondelete = (void*)spider_delete;
	p->onsearch = (void*)spider_search;
	p->onmodify = (void*)spider_modify;

	p->onlinkup = (void*)spider_linkup;
	p->ondiscon = (void*)spider_discon;
	p->ontaking = (void*)spider_taking;
	p->ongiving = (void*)spider_giving;
}
