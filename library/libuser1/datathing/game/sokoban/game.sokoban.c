#include "libuser.h"




static void sokoban_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void sokoban_draw_gl41(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void sokoban_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void sokoban_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void sokoban_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void sokoban_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void sokoban_taking(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void sokoban_giving(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void sokoban_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void sokoban_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void sokoban_search(_obj* act)
{
}
static void sokoban_modify(_obj* act)
{
}
static void sokoban_delete(_obj* act)
{
	if(0 == act)return;
	if(act->listptr.buf0){
		memorydelete(act->listptr.buf0);
		act->listptr.buf0 = 0;
	}
}
static void sokoban_create(_obj* act)
{
	if(0 == act)return;
	//act->buf = memorycreate(WIDTH*HEIGHT, 0);
}




void sokoban_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('s', 'o', 'k', 'o', 'b', 'a', 'n', 0);

	p->oncreate = (void*)sokoban_create;
	p->ondelete = (void*)sokoban_delete;
	p->onsearch = (void*)sokoban_search;
	p->onmodify = (void*)sokoban_modify;

	p->onlinkup = (void*)sokoban_linkup;
	p->ondiscon = (void*)sokoban_discon;
	p->ontaking = (void*)sokoban_taking;
	p->ongiving = (void*)sokoban_giving;
}
