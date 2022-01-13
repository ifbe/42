#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151




static void spider_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void spider_draw_gl41(
	_obj* act, struct style* part,
	_obj* scn, struct style* geom,
	_obj* ctx, struct style* area)
{
}
static void spider_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void spider_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void spider_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void spider_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void spider_wrl_cam_wnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	spider_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void spider_taking(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	//foot defined behavior
	switch(stack[sp-1].foottype){
	}

	//caller defined behavior
	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(caller->hfmt){
	case _rgba_:
		break;
	case _gl41list_:
		break;
	default:
		spider_wrl_cam_wnd(ent,foot, stack,sp);
		break;
	}
}
static void spider_giving(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void spider_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void spider_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void spider_modify(_obj* act)
{
}
static void spider_search(_obj* act)
{
}
static void spider_delete(_obj* act)
{
	if(0 == act)return;
	//if(_copy_ == act->type)memorydelete(act->buf);
}
static void spider_create(_obj* act)
{
	if(0 == act)return;
	//if(_orig_ == act->type)act->buf = buffer;
	//if(_copy_ == act->type)act->buf = memorycreate(256, 0);
}




void spider_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('s', 'p', 'i', 'd', 'e', 'r', 0, 0);

	p->oncreate = (void*)spider_create;
	p->ondelete = (void*)spider_delete;
	p->onsearch = (void*)spider_search;
	p->onmodify = (void*)spider_modify;

	p->onattach = (void*)spider_attach;
	p->ondetach = (void*)spider_detach;
	p->ontaking = (void*)spider_taking;
	p->ongiving = (void*)spider_giving;
}
