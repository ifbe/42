#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151




static void terrian_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->vc[0];
		cy = sty->vc[1];
		ww = sty->vr[0];
		hh = sty->vf[1];
	}
	else
	{
		cx = win->width/2;
		cy = win->height/2;
		ww = win->width/2;
		hh = win->height/2;
	}
}
static void terrian_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	vec3 tc, tr, tf, tu, f;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	carvesolid_rect(win, 0x6a4b23, vc, vr, vf);
}
static void terrian_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void terrian_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void terrian_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void terrian_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void terrian_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)terrian_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)terrian_read_tui(win, sty, act, pin);
	else if(fmt == _html_)terrian_read_html(win, sty, act, pin);
	else if(fmt == _json_)terrian_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)terrian_read_vbo(win, sty, act, pin);
	else terrian_read_pixel(win, sty, act, pin);
}
static void terrian_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void terrian_list()
{
}
static void terrian_change()
{
}
static void terrian_stop(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void terrian_start(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void terrian_delete(struct actor* act)
{
	if(0 == act)return;
	//if(_copy_ == act->type)memorydelete(act->buf);
}
static void terrian_create(struct actor* act)
{
	if(0 == act)return;
	//if(_orig_ == act->type)act->buf = buffer;
	//if(_copy_ == act->type)act->buf = memorycreate(256);
}




void terrian_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('t', 'e', 'r', 'r', 'i', 'a', 'n', 0);

	p->oncreate = (void*)terrian_create;
	p->ondelete = (void*)terrian_delete;
	p->onstart  = (void*)terrian_start;
	p->onstop   = (void*)terrian_stop;
	p->onlist   = (void*)terrian_list;
	p->onchoose = (void*)terrian_change;
	p->onread   = (void*)terrian_read;
	p->onwrite  = (void*)terrian_write;
}
