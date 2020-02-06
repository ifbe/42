#include "libuser.h"




static void editor_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void editor_draw_gl41(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void editor_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void editor_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void editor_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void editor_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	say("editor(%x,%x,%x)\n",win,act,sty);
}




static void editor_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void editor_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void editor_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void editor_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void editor_search(struct entity* act)
{
}
static void editor_modify(struct entity* act)
{
}
static void editor_delete(struct entity* act)
{
	if(0 == act)return;
	if(act->buf0){
		memorydelete(act->buf0);
		act->buf0 = 0;
	}
}
static void editor_create(struct entity* act)
{
	if(0 == act)return;
	act->buf0 = memorycreate(0x1000, 0);
}




void editor_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('e', 'd', 'i', 't', 'o', 'r', 0, 0);

	p->oncreate = (void*)editor_create;
	p->ondelete = (void*)editor_delete;
	p->onsearch = (void*)editor_search;
	p->onmodify = (void*)editor_modify;

	p->onlinkup = (void*)editor_linkup;
	p->ondiscon = (void*)editor_discon;
	p->onread  = (void*)editor_read;
	p->onwrite = (void*)editor_write;
}
