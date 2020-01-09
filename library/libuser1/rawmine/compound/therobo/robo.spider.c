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
	struct entity* wrd, struct style* camg,
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




//-4: wnd, area
//-3: cam, 0
//-2: cam, 0
//-1: world, geom of cam
static void spider_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	//wnd -> cam, cam -> world
	struct entity* wnd;struct style* area;
	struct entity* wrd;struct style* camg;

	//scene -> spider
	struct entity* scn;struct style* geom;
	struct entity* act;struct style* part;

	if(stack){
		act = self->pchip;part = self->pfoot;
		scn = peer->pchip;geom = peer->pfoot;
		wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		if('v' == len)spider_draw_gl41(act,part, scn,geom, wrd,camg, wnd,area);
	}
}
static void spider_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void spider_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void spider_start(struct halfrel* self, struct halfrel* peer)
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

	p->onstart = (void*)spider_start;
	p->onstop  = (void*)spider_stop;
	p->onread  = (void*)spider_read;
	p->onwrite = (void*)spider_write;
}
