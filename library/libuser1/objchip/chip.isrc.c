#include "libuser.h"




static void isrc_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void isrc_read_vbo2d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void isrc_read_vbo3d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void isrc_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void isrc_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void isrc_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void isrc_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void isrc_sread(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)isrc_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)isrc_read_tui(win, sty, act, pin);
	else if(fmt == _html_)isrc_read_html(win, sty, act, pin);
	else if(fmt == _json_)isrc_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)
	{
		if(_2d_ == win->vfmt)isrc_read_vbo2d(win, sty, act, pin);
		else isrc_read_vbo3d(win, sty, act, pin);
	}
	else isrc_read_pixel(win, sty, act, pin);
}
static void isrc_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void isrc_cread(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void isrc_cwrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void isrc_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
}
static void isrc_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
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
	p->onget    = (void*)isrc_cread;
	p->onpost   = (void*)isrc_cwrite;
	p->onread   = (void*)isrc_sread;
	p->onwrite  = (void*)isrc_swrite;
}
