#include "actor.h"
#define PI 3.1415926535897932384626433832795028841971693993151
#define tau (PI*2)
struct plannet
{
	float diameter;
	float distance;
	float period;
	u32 color;
};
static struct plannet data[9] = {
	{1384376.0,          0.0, 999999999.99999, 0xffff00},		//sun
	{   4880.0,   57910000.0,        87.96900, 0xffc200},		//mercury
	{  12103.6,  108200000.0,       224.70000, 0xffc200},		//venus
	{  12756.3,  149600000.0,       365.25636, 0x0000ff},		//earth
	{   6794.0,  227940000.0,       687.00000, 0xff0000},		//mars
	{ 142984.0,  778330000.0,      4332.00000, 0x8a36cf},		//jupiter
	{ 120536.0, 1429400000.0,     10775.00000, 0xf5deb3},		//saturn
	{  51118.0, 2870990000.0,     30681.50000, 0x87ceeb},		//uranus
	{  49532.0, 4504000000.0,     60194.00000, 0x191970}		//neptune
};




static void planet_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u32 c;
	int x,y,l,r,j;
	int cx, cy, ww, hh;
	u64 t = gettime() / 10000;
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

	for(j=0;j<9;j++)
	{
		c = data[j].color;
		if('r' == (win->fmt&0xff))c = ((c>>16)&0xff) + (c&0xff00) + ((c&0xff)<<16);

		l = ww*data[j].distance/data[8].distance;
		r = ww*data[j].diameter/data[8].distance;
		if(j>0)r *= 1024;
		drawline_circle(win, 0x404040, cx, cy, l);

		x = cx + l*cosine(tau*t/data[j].period);
		y = cy + l*sine(tau*t/data[j].period);
		drawsolid_circle(win, c, x, y, r);
	}
}
static void planet_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int j;
	float l, r;
	float a, c, s;
	vec3 tc, tr, tf, tu, f;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	u64 t = gettime() / 10000;

	for(j=0;j<9;j++)
	{
		l = data[j].distance/data[8].distance;
		tr[0] = vr[0]*l;
		tr[1] = vr[1]*l;
		tr[2] = vr[2]*l;
		tu[0] = vu[0]*l;
		tu[1] = vu[1]*l;
		tu[2] = vu[2]*l;
		carveline_circle(win, 0x404040, vc, tr, tu);

		r = data[j].diameter/data[8].distance;
		if(j>0)r *= 1024;
		tr[0] = vr[0]*r;
		tr[1] = vr[1]*r;
		tr[2] = vr[2]*r;
		tf[0] = vf[0]*r;
		tf[1] = vf[1]*r;
		tf[2] = vf[2]*r;
		tu[0] = vu[0]*r;
		tu[1] = vu[1]*r;
		tu[2] = vu[2]*r;

		a = tau*t/data[j].period;
		c = cosine(a);
		s = sine(a);
		tc[0] = vc[0] + (vr[0]*c + vf[0]*s)*l;
		tc[1] = vc[1] + (vr[1]*c + vf[1]*s)*l;
		tc[2] = vc[2] + (vr[2]*c + vf[2]*s)*l;
		carvesolid_sphere(win, data[j].color, tc, tr, tf, tu);
	}
}
static void planet_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void planet_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int len = win->len;
	u8* buf = win->buf;

	len += mysnprintf(
		buf+len, 0x100000-len,
		"<div id=\"planet\" style=\"width:50%%;height:100px;float:left;background-color:#62f4ab;\">"
	);
	len += mysnprintf(buf+len, 0x100000-len, "</div>\n");

	win->len = len;
}
static void planet_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void planet_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void planet_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)planet_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)planet_read_tui(win, sty, act, pin);
	else if(fmt == _html_)planet_read_html(win, sty, act, pin);
	else if(fmt == _json_)planet_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)planet_read_vbo(win, sty, act, pin);
	else planet_read_pixel(win, sty, act, pin);
}
static void planet_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void planet_list()
{
}
static void planet_change()
{
}
static void planet_stop(struct actor* act, struct pinid* pin)
{
}
static void planet_start(struct actor* act, struct pinid* pin)
{
}
static void planet_delete(struct actor* act)
{
	if(0 == act)return;
}
static void planet_create(struct actor* act)
{
	if(0 == act)return;
}




void planet_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('p', 'l', 'a', 'n', 'e', 't', 0, 0);
	p->irel = 0;
	p->orel = 0;

	p->oncreate = (void*)planet_create;
	p->ondelete = (void*)planet_delete;
	p->onstart  = (void*)planet_start;
	p->onstop   = (void*)planet_stop;
	p->onlist   = (void*)planet_list;
	p->onchoose = (void*)planet_change;
	p->onread   = (void*)planet_read;
	p->onwrite  = (void*)planet_write;
}
