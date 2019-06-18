#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151




static void login_draw_pixel(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
}
static void login_draw_vbo(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
	vec3 tc,tr,tf;
	if(0 == sty)sty = defaultstyle_vbo2d();

	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;
	carvesolid2d_rect(win, 0x808080, vc, vr ,vf);

	tc[0] = vc[0] + vf[0]/2;
	tc[1] = vc[1] + vf[1]/2;
	tc[2] = vc[2] + vf[2]/2 + 1.0;
	tr[0] = vr[0] / 4;
	tr[1] = vr[1] / 4;
	tr[2] = vr[2] / 4;
	tf[0] = vf[0] / 4;
	tf[1] = vf[1] / 4;
	tf[2] = vf[2] / 4;
	carvestring2d_center(win, 0xffffff, tc, tr, tf, (void*)"welcome!", 8);

	tc[0] = vc[0] - vf[0]/8;
	tc[1] = vc[1] - vf[1]/8;
	tc[2] = vc[2] - vf[2]/8 + 1.0;
	tr[0] = vr[0] / 4;
	tr[1] = vr[1] / 4;
	tr[2] = vr[2] / 4;
	tf[0] = vf[0] / 16;
	tf[1] = vf[1] / 16;
	tf[2] = vf[2] / 16;
	carvesolid2d_rect(win, 0x202020, tc, tr, tf);
	tc[2] += 1.0;
	tr[0] /= 8;
	tr[1] /= 8;
	tr[1] /= 8;
	carvestring2d_center(win, 0xffffff, tc, tr, tf, (void*)"username", 8);

	tc[0] = vc[0] - vf[0]*3/8;
	tc[1] = vc[1] - vf[1]*3/8;
	tc[2] = vc[2] - vf[2]*3/8 + 1.0;
	tr[0] = vr[0] / 4;
	tr[1] = vr[1] / 4;
	tr[2] = vr[2] / 4;
	tf[0] = vf[0] / 16;
	tf[1] = vf[1] / 16;
	tf[2] = vf[2] / 16;
	carvesolid2d_rect(win, 0x202020, tc, tr, tf);
	tc[2] += 1.0;
	tr[0] /= 8;
	tr[1] /= 8;
	tr[1] /= 8;
	carvestring2d_center(win, 0xffffff, tc, tr, tf, (void*)"password", 8);
}
static void login_draw_json(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
}
static void login_draw_html(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
}
static void login_draw_tui(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
}
static void login_draw_cli(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
}
static void login_draw(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)login_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)login_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)login_draw_html(act, pin, win, sty);
	else if(fmt == _json_)login_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)login_draw_vbo(act, pin, win, sty);
	else login_draw_pixel(act, pin, win, sty);
}




static void login_read(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct arena* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	login_draw(act, pin, win, sty);
}
static void login_write(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void login_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void login_start(struct halfrel* self, struct halfrel* peer)
{
}




static void login_search(struct actor* act)
{
}
static void login_modify(struct actor* act)
{
}
static void login_delete(struct actor* act)
{
	if(0 == act)return;
	//if(_copy_ == act->type)memorydelete(act->buf);
}
static void login_create(struct actor* act)
{
	if(0 == act)return;
	//if(_orig_ == act->type)act->buf = buffer;
	//if(_copy_ == act->type)act->buf = memorycreate(256);
}




void login_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('l', 'o', 'g', 'i', 'n', 0, 0, 0);

	p->oncreate = (void*)login_create;
	p->ondelete = (void*)login_delete;
	p->onsearch = (void*)login_search;
	p->onmodify = (void*)login_modify;

	p->onstart = (void*)login_start;
	p->onstop  = (void*)login_stop;
	p->onread  = (void*)login_read;
	p->onwrite = (void*)login_write;
}
