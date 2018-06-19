#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151




static void water_read_pixel(
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
static void water_read_vbo(
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
static void water_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void water_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void water_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void water_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void water_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)water_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)water_read_tui(win, sty, act, pin);
	else if(fmt == _html_)water_read_html(win, sty, act, pin);
	else if(fmt == _json_)water_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)water_read_vbo(win, sty, act, pin);
	else water_read_pixel(win, sty, act, pin);
}
static void water_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void water_list()
{
}
static void water_change()
{
}
static void water_stop(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void water_start(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void water_delete(struct actor* act)
{
	if(0 == act)return;
	//if(_copy_ == act->type)memorydelete(act->buf);
}
static void water_create(struct actor* act)
{
	if(0 == act)return;
	//if(_orig_ == act->type)act->buf = buffer;
	//if(_copy_ == act->type)act->buf = memorycreate(256);
}




void water_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('w', 'a', 't', 'e', 'r', 0, 0, 0);

	p->oncreate = (void*)water_create;
	p->ondelete = (void*)water_delete;
	p->onstart  = (void*)water_start;
	p->onstop   = (void*)water_stop;
	p->onlist   = (void*)water_list;
	p->onchoose = (void*)water_change;
	p->onread   = (void*)water_read;
	p->onwrite  = (void*)water_write;
}
