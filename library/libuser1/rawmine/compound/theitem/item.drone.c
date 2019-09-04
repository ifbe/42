#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151




static void drone_draw_pixel(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	int cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->f.vc[0];
		cy = sty->f.vc[1];
		ww = sty->f.vr[0];
		hh = sty->f.vf[1];
	}
	else
	{
		cx = win->width/2;
		cy = win->height/2;
		ww = win->width/2;
		hh = win->height/2;
	}
}
static void drone_draw_vbo(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
    vec3 tc,tr,tf,tu;
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;

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
    tf[0] = vf[0] / 32;
    tf[1] = vf[1] / 32;
    tf[2] = vf[2] / 32;
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
    carvesolid_cylinder(win, 0x765432, tc, tr, tf, tu);

    //rb
    tc[0] = vc[0] + rr0 - ff0 + tu[0];
    tc[1] = vc[1] + rr1 - ff1 + tu[1];
    tc[2] = vc[2] + rr2 - ff2 + tu[2];
    carvesolid_cylinder(win, 0x765432, tc, tr, tf, tu);

    //lf
    tc[0] = vc[0] - rr0 + ff0 + tu[0];
    tc[1] = vc[1] - rr1 + ff1 + tu[1];
    tc[2] = vc[2] - rr2 + ff2 + tu[2];
    carvesolid_cylinder(win, 0x765432, tc, tr, tf, tu);

    //rf
    tc[0] = vc[0] + rr0 + ff0 + tu[0];
    tc[1] = vc[1] + rr1 + ff1 + tu[1];
    tc[2] = vc[2] + rr2 + ff2 + tu[2];
    carvesolid_cylinder(win, 0x765432, tc, tr, tf, tu);
}
static void drone_draw_json(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void drone_draw_html(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void drone_draw_tui(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void drone_draw_cli(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void drone_draw(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)drone_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)drone_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)drone_draw_html(act, pin, win, sty);
	else if(fmt == _json_)drone_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)drone_draw_vbo(act, pin, win, sty);
	else drone_draw_pixel(act, pin, win, sty);
}




static void drone_read(struct halfrel* self, struct halfrel* peer, void* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct actor* ctx = buf;
	//say("@drone_read:%llx,%llx,%llx\n",act,win,buf);

	if(ctx){
		if(_gl41data_ == ctx->type)drone_draw_vbo(act,pin,ctx,sty);
	}
}
static void drone_write(struct halfrel* self, struct halfrel* peer, void* buf, int len)
{
}
static void drone_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void drone_start(struct halfrel* self, struct halfrel* peer)
{
}




static void drone_modify(struct actor* act)
{
}
static void drone_search(struct actor* act)
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
	//if(_copy_ == act->type)act->buf = memorycreate(256, 0);
}




void drone_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('d', 'r', 'o', 'n', 'e', 0, 0, 0);

	p->oncreate = (void*)drone_create;
	p->ondelete = (void*)drone_delete;
	p->onsearch = (void*)drone_search;
	p->onmodify = (void*)drone_modify;

	p->onstart = (void*)drone_start;
	p->onstop  = (void*)drone_stop;
	p->onread  = (void*)drone_read;
	p->onwrite = (void*)drone_write;
}
