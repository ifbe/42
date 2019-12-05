#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151




static void otto_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void otto_draw_vbo(
	struct entity* act, struct style* part,
	struct entity* scn, struct style* geom,
	struct entity* wrd, struct style* camg,
	struct entity* ctx, struct style* area)
{
}
static void otto_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void otto_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void otto_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void otto_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void otto_draw(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)otto_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)otto_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)otto_draw_html(act, pin, win, sty);
	else if(fmt == _json_)otto_draw_json(act, pin, win, sty);
	else otto_draw_pixel(act, pin, win, sty);
}




//-4: wnd, area
//-3: cam, 0
//-2: cam, 0
//-1: world, geom of cam
static void otto_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	//wnd -> cam
	struct entity* wnd;struct style* area;

	//cam -> world
	struct entity* wrd;struct style* camg;

	//scene -> otto
	struct entity* scn;struct style* geom;
	struct entity* act;struct style* part;

	if(stack){
		act = self->pchip;part = self->pfoot;
		scn = peer->pchip;geom = peer->pfoot;
		wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		if('v' == len)otto_draw_vbo(act,part, scn,geom, wrd,camg, wnd,area);
	}
}
static void otto_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void otto_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void otto_start(struct halfrel* self, struct halfrel* peer)
{
}




static void otto_modify(struct entity* act)
{
}
static void otto_search(struct entity* act)
{
}
static void otto_delete(struct entity* act)
{
	if(0 == act)return;
	//if(_copy_ == act->type)memorydelete(act->buf);
}
static void otto_create(struct entity* act)
{
	if(0 == act)return;
	//if(_orig_ == act->type)act->buf = buffer;
	//if(_copy_ == act->type)act->buf = memorycreate(256, 0);
}




void otto_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex32('o', 't', 't', 'o');

	p->oncreate = (void*)otto_create;
	p->ondelete = (void*)otto_delete;
	p->onsearch = (void*)otto_search;
	p->onmodify = (void*)otto_modify;

	p->onstart = (void*)otto_start;
	p->onstop  = (void*)otto_stop;
	p->onread  = (void*)otto_read;
	p->onwrite = (void*)otto_write;
}
