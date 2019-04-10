#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151




static void clock_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u32 c[7]={0xff,0xff00,0xffff,0xff0000,0xff00ff,0xffff00,0xffffff};
	int j,k;
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
	u64 date = dateread();
	u8* p = (u8*)&date;

	if(ww < hh)j = ww;
	else j = hh;
	drawsolid_circle(win, 0x222222, cx, cy, j);
	for(j=6;j>=0;j--)
	{
		drawdecimal(win, c[j], cx+64-(j*24), cy-8, p[j]);
	}
}
static void clock_read_vbo2d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u8 j;
	float a,c,s;
	vec3 tc, tr, tf, tu, f;
	if(0 == sty)sty = defaultstyle_vbo2d();

	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	u64 date = dateread();
	u8* p = (u8*)&date;
	carvesolid2d_circle(win, 0x404040, vc, vr, vf);

	a = PI/2 - (p[0]*PI*2.0/60.0);
	c = cosine(a);
	s = sine(a);
	tr[0] = vc[0]+(vr[0]*c+vf[0]*s);
	tr[1] = vc[1]+(vr[1]*c+vf[1]*s);
	tr[2] = vc[2]+(vr[2]*c+vf[2]*s);
	carveline2d(win, 0xff0000, vc, tr);

	a = PI/2 - (p[1]*PI*2.0/60.0);
	c = cosine(a);
	s = sine(a);
	tr[0] = vc[0]+(vr[0]*c+vf[0]*s)*3/4;
	tr[1] = vc[1]+(vr[1]*c+vf[1]*s)*3/4;
	tr[2] = vc[2]+(vr[2]*c+vf[2]*s)*3/4;
	carveline2d(win, 0x00ff00, vc, tr);

	a = PI/2 - (p[2]*PI*2.0/12.0);
	c = cosine(a);
	s = sine(a);
	tr[0] = vc[0]+(vr[0]*c+vf[0]*s)*2/4;
	tr[1] = vc[1]+(vr[1]*c+vf[1]*s)*2/4;
	tr[2] = vc[2]+(vr[2]*c+vf[2]*s)*2/4;
	carveline2d(win, 0x0000ff, vc, tr);

	tr[0] = vr[0]/8;
	tr[1] = vr[1]/8;
	tr[2] = vr[2]/8;
	tf[0] = vf[0]/8;
	tf[1] = vf[1]/8;
	tf[2] = vf[2]/8;
	for(j=0;j<12;j++)
	{
		a = PI/2 - j*PI/6;
		c = cosine(a);
		s = sine(a);
		tc[0] = vc[0] + c*vr[0]*7/8 + s*vf[0]*7/8;
		tc[1] = vc[1] + c*vr[1]*7/8 + s*vf[1]*7/8;
		tc[2] = vc[2] + c*vr[2]*7/8 + s*vf[2]*7/8 - 0.1;
		carve2d_ascii(win, 0xffffff, tc, tr, tf, j<10 ? j+0x30 : j+0x37);
	}
}
static void clock_read_vbo3d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u8 j;
	float a,c,s;
	vec3 tc, tr, tf, tu;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	u64 date = dateread();
	u8* p = (u8*)&date;
	carvesolid_circle(win, 0x404040, vc, vr, vf);

	a = PI/2 - (p[0]*PI*2.0/60.0);
	c = cosine(a);
	s = sine(a);
	tr[0] = vc[0]+(vr[0]*c+vf[0]*s);
	tr[1] = vc[1]+(vr[1]*c+vf[1]*s);
	tr[2] = vc[2]+(vr[2]*c+vf[2]*s);
	carveline(win, 0xff0000, vc, tr);

	a = PI/2 - (p[1]*PI*2.0/60.0);
	c = cosine(a);
	s = sine(a);
	tr[0] = vc[0]+(vr[0]*c+vf[0]*s)*3/4;
	tr[1] = vc[1]+(vr[1]*c+vf[1]*s)*3/4;
	tr[2] = vc[2]+(vr[2]*c+vf[2]*s)*3/4;
	carveline(win, 0x00ff00, vc, tr);

	a = PI/2 - (p[2]*PI*2.0/12.0);
	c = cosine(a);
	s = sine(a);
	tr[0] = vc[0]+(vr[0]*c+vf[0]*s)*2/4;
	tr[1] = vc[1]+(vr[1]*c+vf[1]*s)*2/4;
	tr[2] = vc[2]+(vr[2]*c+vf[2]*s)*2/4;
	carveline(win, 0x0000ff, vc, tr);

	tr[0] = vr[0]/8;
	tr[1] = vr[1]/8;
	tr[2] = vr[2]/8;
	tf[0] = vf[0]/8;
	tf[1] = vf[1]/8;
	tf[2] = vf[2]/8;
	for(j=0;j<12;j++)
	{
		a = PI/2 - j*PI/6;
		c = cosine(a);
		s = sine(a);
		tc[0] = vc[0] + c*vr[0]*7/8 + s*vf[0]*7/8;
		tc[1] = vc[1] + c*vr[1]*7/8 + s*vf[1]*7/8;
		tc[2] = vc[2] + c*vr[2]*7/8 + s*vf[2]*7/8 + 1;
		carveascii(win, 0xffffff, tc, tr, tf, j<10 ? j+0x30 : j+0x37);
	}
}
static void clock_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void clock_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int len = win->len;
	u8* buf = win->buf;

	len += mysnprintf(
		buf+len, 0x100000-len,
		"<div id=\"clock\" style=\"width:50%%;height:100px;float:left;background-color:#7c89da;\">"
	);
	len += mysnprintf(buf+len, 0x100000-len, "</div>\n");

	win->len = len;
}
static void clock_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void clock_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 date = dateread();
	u8* p = (u8*)&date;

	say(	"_%02d_%02d_%02d_%02d_%02d_%02d_%02d\n",
		p[6],p[5],p[4],p[3],p[2],p[1],p[0]);
}
static void clock_sread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)clock_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)clock_read_tui(win, sty, act, pin);
	else if(fmt == _html_)clock_read_html(win, sty, act, pin);
	else if(fmt == _json_)clock_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)
	{
		if(_2d_ == win->vfmt)clock_read_vbo2d(win, sty, act, pin);
		else clock_read_vbo3d(win, sty, act, pin);
	}
	else clock_read_pixel(win, sty, act, pin);
}
static void clock_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void clock_cread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void clock_cwrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void clock_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
}
static void clock_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
}
static void clock_delete(struct actor* act)
{
	if(0 == act)return;
	//if(_copy_ == act->type)memorydelete(act->buf);
}
static void clock_create(struct actor* act)
{
	if(0 == act)return;
	//if(_orig_ == act->type)act->buf = buffer;
	//if(_copy_ == act->type)act->buf = memorycreate(256);
}




void clock_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('c', 'l', 'o', 'c', 'k', 0, 0, 0);

	p->oncreate = (void*)clock_create;
	p->ondelete = (void*)clock_delete;
	p->onstart  = (void*)clock_start;
	p->onstop   = (void*)clock_stop;
	p->oncread  = (void*)clock_cread;
	p->oncwrite = (void*)clock_cwrite;
	p->onsread  = (void*)clock_sread;
	p->onswrite = (void*)clock_swrite;
}
