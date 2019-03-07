#include "libuser.h"
u32 getrandom();




static void example_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u32 bg,fg;
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

	bg = getrandom()&0xffffff;
	fg = (~bg)&0xffffff;
	drawsolid_rect(win, bg, cx-ww, cy-hh, cx+ww, cy+hh);
	drawhexadecimal(win, fg, cx, cy, bg);
}
static void example_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	vec3 tc, tr, tf, tu, f;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;

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
static void example_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void example_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
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
static void example_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void example_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void example_sread(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)example_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)example_read_tui(win, sty, act, pin);
	else if(fmt == _html_)example_read_html(win, sty, act, pin);
	else if(fmt == _json_)example_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)example_read_vbo(win, sty, act, pin);
	else example_read_pixel(win, sty, act, pin);
}
static void example_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void example_cread(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void example_cwrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void example_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
}
static void example_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
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
	p->name = hex64('e', 'x', 'a', 'm', 'p', 'l', 'e', 0);

	p->oncreate = (void*)example_create;
	p->ondelete = (void*)example_delete;
	p->onstart  = (void*)example_start;
	p->onstop   = (void*)example_stop;
	p->onget    = (void*)example_cread;
	p->onpost   = (void*)example_cread;
	p->onread   = (void*)example_sread;
	p->onwrite  = (void*)example_swrite;
}
