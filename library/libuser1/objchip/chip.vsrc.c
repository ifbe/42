#include "libuser.h"




static void vsrc_draw_pixel(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
}
static void vsrc_draw_vbo2d(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
}
static void vsrc_draw_vbo3d(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
}
static void vsrc_draw_json(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
}
static void vsrc_draw_html(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
}
static void vsrc_draw_tui(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
}
static void vsrc_draw_cli(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
}
static void vsrc_draw(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)vsrc_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)vsrc_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)vsrc_draw_html(act, pin, win, sty);
	else if(fmt == _json_)vsrc_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)
	{
		if(_2d_ == win->vfmt)vsrc_draw_vbo2d(act, pin, win, sty);
		else vsrc_draw_vbo3d(act, pin, win, sty);
	}
	else vsrc_draw_pixel(act, pin, win, sty);
}




static void vsrc_sread(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct pinid* pin = (void*)(self->foot);
	struct arena* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	vsrc_draw(act, pin, win, sty);
}
static void vsrc_swrite(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void vsrc_cread(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void vsrc_cwrite(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void vsrc_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void vsrc_start(struct halfrel* self, struct halfrel* peer)
{
}
static void vsrc_delete(struct actor* act, u8* buf)
{
}
static void vsrc_create(struct actor* act, u8* buf)
{
}




void vsrc_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex32('v','s','r','c');

	p->oncreate = (void*)vsrc_create;
	p->ondelete = (void*)vsrc_delete;
	p->onstart  = (void*)vsrc_start;
	p->onstop   = (void*)vsrc_stop;
	p->oncread  = (void*)vsrc_cread;
	p->oncwrite = (void*)vsrc_cwrite;
	p->onsread  = (void*)vsrc_sread;
	p->onswrite = (void*)vsrc_swrite;
}
