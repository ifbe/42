#include "libuser.h"




static void chippin_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void chippin_read_vbo2d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void chippin_read_vbo3d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void chippin_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void chippin_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void chippin_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void chippin_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void chippin_sread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)chippin_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)chippin_read_tui(win, sty, act, pin);
	else if(fmt == _html_)chippin_read_html(win, sty, act, pin);
	else if(fmt == _json_)chippin_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)
	{
		if(_2d_ == win->vfmt)chippin_read_vbo2d(win, sty, act, pin);
		else chippin_read_vbo3d(win, sty, act, pin);
	}
	else chippin_read_pixel(win, sty, act, pin);
}
static void chippin_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void chippin_cread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void chippin_cwrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void chippin_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
}
static void chippin_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
}
static void chippin_delete(struct actor* act, u8* buf)
{
}
static void chippin_create(struct actor* act, u8* buf)
{
}




void chippin_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex32('p','i','n',0);

	p->oncreate = (void*)chippin_create;
	p->ondelete = (void*)chippin_delete;
	p->onstart  = (void*)chippin_start;
	p->onstop   = (void*)chippin_stop;
	p->oncread  = (void*)chippin_cread;
	p->oncwrite = (void*)chippin_cread;
	p->onsread  = (void*)chippin_sread;
	p->onswrite = (void*)chippin_swrite;
}
