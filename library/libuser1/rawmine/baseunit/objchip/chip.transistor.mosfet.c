#include "libuser.h"




static void mosfet_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void mosfet_draw_vbo3d(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	int j;
	vec3 tc,tr,tf,tu;
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;
}
static void mosfet_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void mosfet_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void mosfet_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void mosfet_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void mosfet_draw(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)mosfet_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)mosfet_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)mosfet_draw_html(act, pin, win, sty);
	else if(fmt == _json_)mosfet_draw_json(act, pin, win, sty);
	else mosfet_draw_pixel(act, pin, win, sty);
}




static void mosfet_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
}
static void mosfet_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
}
static void mosfet_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void mosfet_start(struct halfrel* self, struct halfrel* peer)
{
}




static void mosfet_search(struct entity* act, u8* buf)
{
}
static void mosfet_modify(struct entity* act, u8* buf)
{
}
static void mosfet_delete(struct entity* act, u8* buf)
{
}
static void mosfet_create(struct entity* act, u8* buf)
{
}




void mosfet_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('m','o','s','f','e','t', 0, 0);

	p->oncreate = (void*)mosfet_create;
	p->ondelete = (void*)mosfet_delete;
	p->onsearch = (void*)mosfet_search;
	p->onmodify = (void*)mosfet_modify;

	p->onstart = (void*)mosfet_start;
	p->onstop  = (void*)mosfet_stop;
	p->onread  = (void*)mosfet_read;
	p->onwrite = (void*)mosfet_write;
}
