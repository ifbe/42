#include "libuser.h"




static void resistor_draw_pixel(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
}
static void resistor_draw_vbo2d(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
}
static void resistor_draw_vbo3d(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
}
static void resistor_draw_json(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
}
static void resistor_draw_html(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
}
static void resistor_draw_tui(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
}
static void resistor_draw_cli(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
}
static void resistor_draw(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)resistor_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)resistor_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)resistor_draw_html(act, pin, win, sty);
	else if(fmt == _json_)resistor_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)
	{
		if(_2d_ == win->vfmt)resistor_draw_vbo2d(act, pin, win, sty);
		else resistor_draw_vbo3d(act, pin, win, sty);
	}
	else resistor_draw_pixel(act, pin, win, sty);
}




static void resistor_sread(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct arena* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	resistor_draw(act, pin, win, sty);
}
static void resistor_swrite(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void resistor_cread(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void resistor_cwrite(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void resistor_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void resistor_start(struct halfrel* self, struct halfrel* peer)
{
}
static void resistor_delete(struct actor* act, u8* buf)
{
}
static void resistor_create(struct actor* act, u8* buf)
{
}




void resistor_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('r','e','s','i','s','t','o','r');

	p->oncreate = (void*)resistor_create;
	p->ondelete = (void*)resistor_delete;
	p->onstart  = (void*)resistor_start;
	p->onstop   = (void*)resistor_stop;
	p->oncread  = (void*)resistor_cread;
	p->oncwrite = (void*)resistor_cwrite;
	p->onsread  = (void*)resistor_sread;
	p->onswrite = (void*)resistor_swrite;
}
