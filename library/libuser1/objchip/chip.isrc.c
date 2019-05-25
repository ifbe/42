#include "libuser.h"




static void isrc_draw_pixel(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
}
static void isrc_draw_vbo2d(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
}
static void isrc_draw_vbo3d(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
}
static void isrc_draw_json(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
}
static void isrc_draw_html(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
}
static void isrc_draw_tui(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
}
static void isrc_draw_cli(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
}
static void isrc_draw(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)isrc_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)isrc_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)isrc_draw_html(act, pin, win, sty);
	else if(fmt == _json_)isrc_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)
	{
		if(_2d_ == win->vfmt)isrc_draw_vbo2d(act, pin, win, sty);
		else isrc_draw_vbo3d(act, pin, win, sty);
	}
	else isrc_draw_pixel(act, pin, win, sty);
}




static void isrc_sread(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct arena* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	isrc_draw(act, pin, win, sty);
}
static void isrc_swrite(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void isrc_cread(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void isrc_cwrite(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void isrc_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void isrc_start(struct halfrel* self, struct halfrel* peer)
{
}
static void isrc_delete(struct actor* act, u8* buf)
{
}
static void isrc_create(struct actor* act, u8* buf)
{
}




void isrc_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex32('i','s','r','c');

	p->oncreate = (void*)isrc_create;
	p->ondelete = (void*)isrc_delete;
	p->onstart  = (void*)isrc_start;
	p->onstop   = (void*)isrc_stop;
	p->oncread  = (void*)isrc_cread;
	p->oncwrite = (void*)isrc_cwrite;
	p->onsread  = (void*)isrc_sread;
	p->onswrite = (void*)isrc_swrite;
}
