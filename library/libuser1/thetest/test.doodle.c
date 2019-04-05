#include "libuser.h"
double arctan2(double,double);
double cosine(double);
double sine(double);



static int px=0;
static int py=0;
static u8 buffer[16];




void doodle_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	float c,s,f;
	int x0,y0,x1,y1;
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

	//circle
	drawsolid_circle(win, 0x00ffff, cx, cy, ww);
	drawsolid_circle(win, 0x404040, cx+ww/2, cy, ww/2);
	drawsolid_circle(win, 0x404040, cx-ww/2, cy, ww/2);

	f = arctan2(py-cy, px-cx+(ww/2));
	c = cosine(f);
	s = sine(f);
	x0 = (int)(c*ww/4) + (cx-ww/2);
	y0 = (int)(s*ww/4) + cy;

	f = arctan2(py-cy, px-cx-(ww/2));
	c = cosine(f);
	s = sine(f);
	x1 = (int)(c*ww/4) + (cx+ww/2);
	y1 = (int)(s*ww/4) + cy;

	drawsolid_circle(win, 0xff0000, x0, y0, ww/4);
	drawsolid_circle(win, 0xff0000, x1, y1, ww/4);

	drawline(win, 0xffffff, x0, y0, px, py);
	drawline(win, 0xffffff, x1, y1, px, py);

	drawbezier(win, 0xffffff, x0, y0, x1, y1, px, py);
}
static void doodle_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	float a,c,s;
	vec3 tc, tr, tf, tu, f;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	carvesolid_circle(win, 0x00ffff, vc, vr, vf);

	tr[0] = vr[0]/2;
	tr[1] = vr[1]/2;
	tr[2] = vr[2]/2;
	tf[0] = vf[0]/2;
	tf[1] = vf[1]/2;
	tf[2] = vf[2]/2;

	tc[0] = vc[0]-vr[0]/2+vu[0]/8;
	tc[1] = vc[1]-vr[1]/2+vu[1]/8;
	tc[2] = vc[2]-vr[2]/2+vu[2]/8,
	carvesolid_circle(win, 0x404040, tc, tr, tf);

	tc[0] = vc[0]+vr[0]/2+vu[0]/8;
	tc[1] = vc[1]+vr[1]/2+vu[1]/8;
	tc[2] = vc[2]+vr[2]/2+vu[2]/8;
	carvesolid_circle(win, 0x404040, tc, tr, tf);

	tr[0] = vr[0]/4;
	tr[1] = vr[1]/4;
	tr[2] = vr[2]/4;
	tf[0] = vf[0]/4;
	tf[1] = vf[1]/4;
	tf[2] = vf[2]/4;

	a = arctan2(py-vc[1]+vr[1], px-vc[0]+vr[0]);
	c = cosine(a);
	s = sine(a);
	tc[0] = vc[0]-vr[0]/2+(vr[0]*c+vf[0]*s)/4+vu[0]/4;
	tc[1] = vc[1]-vr[1]/2+(vr[1]*c+vf[1]*s)/4+vu[1]/4;
	tc[2] = vc[2]-vr[2]/2+(vr[2]*c+vf[2]*s)/4+vu[2]/4;
	carvesolid_circle(win, 0xff0000, tc, tr, tf);

	tu[0] = px;
	tu[1] = py;
	tu[2] = 0.0;
	carveline(win, 0xffffff, tc, tu);

	a = arctan2(py-vc[1]-vr[0], px-vc[0]-vr[1]);
	c = cosine(a);
	s = sine(a);
	tc[0] = vc[0]+vr[0]/2+(vr[0]*c+vf[0]*s)/4+vu[0]/4;
	tc[1] = vc[1]+vr[1]/2+(vr[1]*c+vf[1]*s)/4+vu[1]/4;
	tc[2] = vc[2]+vr[2]/2+(vr[2]*c+vf[2]*s)/4+vu[2]/4;
	carvesolid_circle(win, 0xff0000, tc, tr, tf);

	tu[0] = px;
	tu[1] = py;
	tu[2] = 0.0;
	carveline(win, 0xffffff, tc, tu);
}
static void doodle_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void doodle_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int len = win->len;
	u8* buf = win->buf;

	len += mysnprintf(
		buf+len, 0x100000-len,
		"<div id=\"doodle\" style=\"width:50%%;height:100px;float:left;background-color:#28a779;\">"
	);
	len += mysnprintf(buf+len, 0x100000-len, "</div>\n");

	win->len = len;
}
static void doodle_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void doodle_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	say("doodle(%x,%x,%x)\n", win, sty, act, pin);
}
static void doodle_sread(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)doodle_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)doodle_read_tui(win, sty, act, pin);
	else if(fmt == _html_)doodle_read_html(win, sty, act, pin);
	else if(fmt == _json_)doodle_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)doodle_read_vbo(win, sty, act, pin);
	else doodle_read_pixel(win, sty, act, pin);
}
static void doodle_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	u64 what = ev->what;
	u64 why = ev->why;
	if(0x4070 == what)
	{
		px = why&0xffff;
		py = (why>>16)&0xffff;
	}
}
static void doodle_cread(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void doodle_cwrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void doodle_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
}
static void doodle_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
}
static void doodle_delete(struct actor* act)
{
}
static void doodle_create(struct actor* act)
{
	if(0 == act)return;
	act->buf = ((void*)act) + 0x100;
}




void doodle_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('d', 'o', 'o', 'd', 'l', 'e', 0, 0);

	p->oncreate = (void*)doodle_create;
	p->ondelete = (void*)doodle_delete;
	p->onstart  = (void*)doodle_start;
	p->onstop   = (void*)doodle_stop;
	p->onget    = (void*)doodle_cread;
	p->onpost   = (void*)doodle_cwrite;
	p->onread   = (void*)doodle_sread;
	p->onwrite  = (void*)doodle_swrite;
}
