#include "libuser.h"
u32 getrandom();




static void example_draw_pixel(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
	u32 bg,fg;
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

	bg = getrandom()&0xffffff;
	fg = (~bg)&0xffffff;
	drawsolid_rect(win, bg, cx-ww, cy-hh, cx+ww, cy+hh);
	drawhexadecimal(win, fg, cx, cy, bg);
}
static void example_draw_vbo(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
	vec3 tc, tr, tf, tu, f;
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;

	tr[0] = vr[0]/2;
	tr[1] = vr[1]/2;
	tr[2] = vr[2]/2;
	tf[0] = vf[0]/2;
	tf[1] = vf[1]/2;
	tf[2] = vf[2]/2;
	tu[0] = vu[0]/2;
	tu[1] = vu[1]/2;
	tu[2] = vu[2]/2;

	tc[0] = vc[0]-vr[0]/2-vf[0]/2+vu[0]/2;
	tc[1] = vc[1]-vr[1]/2-vf[1]/2+vu[1]/2;
	tc[2] = vc[2]-vr[2]/2-vf[2]/2+vu[2]/2,
	carvepoint_sphere(win, 0xffffff, tc, tr, tf, tu);

	tc[0] = vc[0]+vr[0]/2-vf[0]/2+vu[0]/2;
	tc[1] = vc[1]+vr[1]/2-vf[1]/2+vu[1]/2;
	tc[2] = vc[2]+vr[2]/2-vf[2]/2+vu[2]/2;
	carveline_sphere(win, 0xffffff, tc, tr, tf, tu);

	tc[0] = vc[0]-vr[0]/2+vf[0]/2+vu[0]/2;
	tc[1] = vc[1]-vr[1]/2+vf[1]/2+vu[1]/2;
	tc[2] = vc[2]-vr[2]/2+vf[2]/2+vu[2]/2;
	carvesolid_sphere(win, 0xff00ff, tc, tr, tf, tu);

	tc[0] = vc[0]-vr[0]/2-vf[0]/2+vu[0]*3/2;
	tc[1] = vc[1]-vr[1]/2-vf[1]/2+vu[1]*3/2;
	tc[2] = vc[2]-vr[2]/2-vf[2]/2+vu[2]*3/2;
	carvepoint_sphere(win, 0xffffff, tc, tr, tf, tu);

	tc[0] = vc[0]+vr[0]/2-vf[0]/2+vu[0]*3/2;
	tc[1] = vc[1]+vr[1]/2-vf[1]/2+vu[1]*3/2;
	tc[2] = vc[2]+vr[2]/2-vf[2]/2+vu[2]*3/2;
	carveline_sphere(win, 0xffffff, tc, tr, tf, tu);

	tc[0] = vc[0]-vr[0]/2+vf[0]/2+vu[0]*3/2;
	tc[1] = vc[1]-vr[1]/2+vf[1]/2+vu[1]*3/2;
	tc[2] = vc[2]-vr[2]/2+vf[2]/2+vu[2]*3/2;
	carvesolid_sphere(win, 0x87cefa, tc, tr, tf, tu);
}
static void example_draw_json(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
}
static void example_draw_html(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
	int len = win->len;
	u8* buf = win->buf;

	len += mysnprintf(
		buf+len, 0x100000-len,
		"<div id=\"example\" style=\"width:50%%;height:100px;float:left;background-color:#82a977;\">"
	);
	len += mysnprintf(buf+len, 0x100000-len, "</div>\n");

	win->len = len;
}
static void example_draw_tui(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
}
static void example_draw_cli(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
}
static void example_draw(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)example_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)example_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)example_draw_html(act, pin, win, sty);
	else if(fmt == _json_)example_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)example_draw_vbo(act, pin, win, sty);
	else example_draw_pixel(act, pin, win, sty);
}




static void example_read(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct arena* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	example_draw(act, pin, win, sty);
}
static void example_write(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void example_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void example_start(struct halfrel* self, struct halfrel* peer)
{
}




static void example_search(struct actor* act)
{
}
static void example_modify(struct actor* act)
{
}
static void example_delete(struct actor* act)
{
	if(0 == act)return;
}
static void example_create(struct actor* act)
{
	if(0 == act)return;
}




void example_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('e', 'x', 'a', 'm', 'p', 'l', 'e', 0);

	p->oncreate = (void*)example_create;
	p->ondelete = (void*)example_delete;
	p->onsearch = (void*)example_search;
	p->onmodify = (void*)example_modify;

	p->onstart = (void*)example_start;
	p->onstop  = (void*)example_stop;
	p->onread  = (void*)example_read;
	p->onwrite = (void*)example_read;
}
