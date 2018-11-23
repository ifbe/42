#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151




static void drone_read_pixel(
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
static void drone_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
    vec3 tc,tr,tf,tu;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;

    tu[0] = vu[0] / 64;
    tu[1] = vu[1] / 64;
    tu[2] = vu[2] / 64;

    //center
    tr[0] = vr[0] / 4;
    tr[1] = vr[1] / 4;
    tr[2] = vr[2] / 4;
    tf[0] = vf[0] / 4;
    tf[1] = vf[1] / 4;
    tf[2] = vf[2] / 4;
    carvesolid_prism4(win, 0xffffff, vc, tr, tf, tu);

    //pie
    tr[0] = vr[0] + vf[0];
    tr[1] = vr[1] + vf[1];
    tr[2] = vr[2] + vf[2];
    tf[0] = (vf[0] - vr[0]) / 16;
    tf[1] = (vf[1] - vr[1]) / 16;
    tf[2] = (vf[2] - vr[2]) / 16;
    carvesolid_prism4(win, 0xfedcba, vc, tr, tf, tu);

    //na
    tr[0] = (vr[0] + vf[0]) / 16;
    tr[1] = (vr[1] + vf[1]) / 16;
    tr[2] = (vr[2] + vf[2]) / 16;
    tf[0] = vf[0] - vr[0];
    tf[1] = vf[1] - vr[1];
    tf[2] = vf[2] - vr[2];
    carvesolid_prism4(win, 0xfedcba, vc, tr, tf, tu);


    tr[0] = vr[0] / 32;
    tr[1] = vr[1] / 32;
    tr[2] = vr[2] / 32;
    tu[0] = vu[0] / 32;
    tu[1] = vu[1] / 32;
    tu[2] = vu[2] / 32;
#define rr0 (vr[0]*31/32)
#define rr1 (vr[1]*31/32)
#define rr2 (vr[2]*31/32)
#define ff0 (vf[0]*31/32)
#define ff1 (vf[1]*31/32)
#define ff2 (vf[2]*31/32)

    //lb
    tc[0] = vc[0] - rr0 - ff0 + tu[0];
    tc[1] = vc[1] - rr1 - ff1 + tu[1];
    tc[2] = vc[2] - rr2 - ff2 + tu[2];
    carvesolid_cylinder(win, 0x765432, tc, tr, tu);

    //rb
    tc[0] = vc[0] + rr0 - ff0 + tu[0];
    tc[1] = vc[1] + rr1 - ff1 + tu[1];
    tc[2] = vc[2] + rr2 - ff2 + tu[2];
    carvesolid_cylinder(win, 0x765432, tc, tr, tu);

    //lf
    tc[0] = vc[0] - rr0 + ff0 + tu[0];
    tc[1] = vc[1] - rr1 + ff1 + tu[1];
    tc[2] = vc[2] - rr2 + ff2 + tu[2];
    carvesolid_cylinder(win, 0x765432, tc, tr, tu);

    //rf
    tc[0] = vc[0] + rr0 + ff0 + tu[0];
    tc[1] = vc[1] + rr1 + ff1 + tu[1];
    tc[2] = vc[2] + rr2 + ff2 + tu[2];
    carvesolid_cylinder(win, 0x765432, tc, tr, tu);
}
static void drone_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void drone_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void drone_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void drone_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void drone_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)drone_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)drone_read_tui(win, sty, act, pin);
	else if(fmt == _html_)drone_read_html(win, sty, act, pin);
	else if(fmt == _json_)drone_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)drone_read_vbo(win, sty, act, pin);
	else drone_read_pixel(win, sty, act, pin);
}
static void drone_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void drone_get()
{
}
static void drone_post()
{
}
static void drone_stop(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void drone_start(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void drone_delete(struct actor* act)
{
	if(0 == act)return;
	//if(_copy_ == act->type)memorydelete(act->buf);
}
static void drone_create(struct actor* act)
{
	if(0 == act)return;
	//if(_orig_ == act->type)act->buf = buffer;
	//if(_copy_ == act->type)act->buf = memorycreate(256);
}




void drone_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('d', 'r', 'o', 'n', 'e', 0, 0, 0);

	p->oncreate = (void*)drone_create;
	p->ondelete = (void*)drone_delete;
	p->onstart  = (void*)drone_start;
	p->onstop   = (void*)drone_stop;
	p->onget    = (void*)drone_get;
	p->onpost   = (void*)drone_post;
	p->onread   = (void*)drone_read;
	p->onwrite  = (void*)drone_write;
}
