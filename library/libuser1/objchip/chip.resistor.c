#include "libuser.h"




static void resistor_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void resistor_read_vbo2d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void resistor_read_vbo3d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void resistor_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void resistor_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void resistor_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void resistor_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void resistor_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)resistor_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)resistor_read_tui(win, sty, act, pin);
	else if(fmt == _html_)resistor_read_html(win, sty, act, pin);
	else if(fmt == _json_)resistor_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)
	{
		if(_2d_ == win->vfmt)resistor_read_vbo2d(win, sty, act, pin);
		else resistor_read_vbo3d(win, sty, act, pin);
	}
	else resistor_read_pixel(win, sty, act, pin);
}




static void resistor_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void resistor_post(u8* buf, int len)
{
}
static void resistor_get(u8* buf, int len)
{
}
static void resistor_stop(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void resistor_start(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
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
	p->name = hex64('r','e','s','i','s','t','o','r');

	p->oncreate = (void*)resistor_create;
	p->ondelete = (void*)resistor_delete;
	p->onstart  = (void*)resistor_start;
	p->onstop   = (void*)resistor_stop;
	p->onget    = (void*)resistor_get;
	p->onpost   = (void*)resistor_post;
	p->onread   = (void*)resistor_read;
	p->onwrite  = (void*)resistor_write;
}
