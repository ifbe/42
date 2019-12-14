#include "libuser.h"




static void chippin_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void chippin_draw_vbo3d(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void chippin_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void chippin_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void chippin_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void chippin_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void chippin_draw(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)chippin_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)chippin_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)chippin_draw_html(act, pin, win, sty);
	else if(fmt == _json_)chippin_draw_json(act, pin, win, sty);
	else chippin_draw_pixel(act, pin, win, sty);
}




static void chippin_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	//if 'draw' == self.foot
	struct entity* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct entity* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	//chippin_draw(act, pin, win, sty);
}
static void chippin_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
}
static void chippin_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void chippin_start(struct halfrel* self, struct halfrel* peer)
{
}




static void chippin_search(struct entity* act, u8* buf)
{
}
static void chippin_modify(struct entity* act, u8* buf)
{
}
static void chippin_delete(struct entity* act, u8* buf)
{
}
static void chippin_create(struct entity* act, u8* buf)
{
}




void chippin_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex32('p','i','n',0);

	p->oncreate = (void*)chippin_create;
	p->ondelete = (void*)chippin_delete;
	p->onsearch = (void*)chippin_search;
	p->onmodify = (void*)chippin_modify;

	p->onstart = (void*)chippin_start;
	p->onstop  = (void*)chippin_stop;
	p->onread  = (void*)chippin_read;
	p->onwrite = (void*)chippin_write;
}
