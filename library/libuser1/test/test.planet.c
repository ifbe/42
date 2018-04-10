#include "actor.h"
#define PI 3.1415926535897932384626433832795028841971693993151
#define tau (PI*2)
#define diameter_sun 1384376.0
#define diameter_mercury   4880.0
#define diameter_venus    12103.6
#define diameter_earth    12756.3
#define diameter_mars      6794.0
#define diameter_jupiter 142984.0
#define diameter_saturn  120536.0
#define diameter_uranus   51118.0
#define diameter_neptune  49532.0
#define distance_mercury   57910000.0
#define distance_venus    108200000.0
#define distance_earth    149600000.0
#define distance_mars     227940000.0
#define distance_jupiter  778330000.0
#define distance_saturn  1429400000.0
#define distance_uranus  2870990000.0
#define distance_neptune 4504000000.0
#define period_mercury    87.969
#define period_venus     224.7
#define period_earth     365.25636
#define period_mars      687.0
#define period_jupiter  4332.0
#define period_saturn  10775.0
#define period_uranus  30681.5
#define period_neptune 60194.0
u32 color[9] = {0xffff00,
	0xffc200, 0xffc200, 0x0000ff, 0xff0000,
	0x8a36cf, 0xf5deb3, 0x87ceeb, 0x191970
};




static void planet_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u32 c;
	int x,y,l,r;
	int cx = sty->cx;
	int cy = sty->cy;
	int ww = sty->rx;
	int hh = sty->fy;
	u64 t = gettime() / 10000;

	c = color[0];
	if('r' == (win->fmt&0xff))c = ((c>>16)&0xff) + (c&0xff00) + ((c&0xff)<<16);
	r = ww*diameter_sun/distance_neptune;
	x = cx;
	y = cy;
	drawsolid_circle(win, c, x, y, r);

	c = color[1];
	if('r' == (win->fmt&0xff))c = ((c>>16)&0xff) + (c&0xff00) + ((c&0xff)<<16);
	l = ww*distance_mercury/distance_neptune;
	r = 1024*ww*diameter_mercury/distance_neptune;
	drawline_circle(win, 0x404040, cx, cy, l);
	x = cx + l*cosine(tau*t/period_mercury);
	y = cy + l*sine(tau*t/period_mercury);
	drawsolid_circle(win, c, x, y, r);

	c = color[2];
	if('r' == (win->fmt&0xff))c = ((c>>16)&0xff) + (c&0xff00) + ((c&0xff)<<16);
	l = ww*distance_venus/distance_neptune;
	r = 1024*ww*diameter_venus/distance_neptune;
	drawline_circle(win, 0x404040, cx, cy, l);
	x = cx + l*cosine(tau*t/period_venus);
	y = cy + l*sine(tau*t/period_venus);
	drawsolid_circle(win, c, x, y, r);

	c = color[3];
	if('r' == (win->fmt&0xff))c = ((c>>16)&0xff) + (c&0xff00) + ((c&0xff)<<16);
	l = ww*distance_earth/distance_neptune;
	r = 1024*ww*diameter_earth/distance_neptune;
	drawline_circle(win, 0x404040, cx, cy, l);
	x = cx + l*cosine(tau*t/period_earth);
	y = cy + l*sine(tau*t/period_earth);
	drawsolid_circle(win, c, x, y, r);

	c = color[4];
	if('r' == (win->fmt&0xff))c = ((c>>16)&0xff) + (c&0xff00) + ((c&0xff)<<16);
	l = ww*distance_mars/distance_neptune;
	r = 1024*ww*diameter_mars/distance_neptune;
	drawline_circle(win, 0x404040, cx, cy, l);
	x = cx + l*cosine(tau*t/period_mars);
	y = cy + l*sine(tau*t/period_mars);
	drawsolid_circle(win, c, x, y, r);

	c = color[5];
	if('r' == (win->fmt&0xff))c = ((c>>16)&0xff) + (c&0xff00) + ((c&0xff)<<16);
	l = ww*distance_jupiter/distance_neptune;
	r = 1024*ww*diameter_jupiter/distance_neptune;
	drawline_circle(win, 0x404040, cx, cy, l);
	x = cx + l*cosine(tau*t/period_jupiter);
	y = cy + l*sine(tau*t/period_jupiter);
	drawsolid_circle(win, c, x, y, r);

	c = color[6];
	if('r' == (win->fmt&0xff))c = ((c>>16)&0xff) + (c&0xff00) + ((c&0xff)<<16);
	l = ww*distance_saturn/distance_neptune;
	r = 1024*ww*diameter_saturn/distance_neptune;
	drawline_circle(win, 0x404040, cx, cy, l);
	x = cx + l*cosine(tau*t/period_saturn);
	y = cy + l*sine(tau*t/period_saturn);
	drawsolid_circle(win, c, x, y, r);

	c = color[7];
	if('r' == (win->fmt&0xff))c = ((c>>16)&0xff) + (c&0xff00) + ((c&0xff)<<16);
	l = ww*distance_uranus/distance_neptune;
	r = 1024*ww*diameter_uranus/distance_neptune;
	drawline_circle(win, 0x404040, cx, cy, l);
	x = cx + l*cosine(tau*t/period_uranus);
	y = cy + l*sine(tau*t/period_uranus);
	drawsolid_circle(win, c, x, y, r);

	c = color[8];
	if('r' == (win->fmt&0xff))c = ((c>>16)&0xff) + (c&0xff00) + ((c&0xff)<<16);
	l = ww*distance_neptune/distance_neptune;
	r = 1024*ww*diameter_neptune/distance_neptune;
	drawline_circle(win, 0x404040, cx, cy, l);
	x = cx + l*cosine(tau*t/period_neptune);
	y = cy + l*sine(tau*t/period_neptune);
	drawsolid_circle(win, c, x, y, r);
}
static void planet_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	float x, y, l, r;
	float cx = sty->cx;
	float cy = sty->cy;
	float cz = sty->cz;
	float ww = sty->rx;
	float hh = sty->fy;
	float dd = sty->uz;
	u64 t = gettime() / 10000;

	r = ww*diameter_sun/distance_neptune;
	x = cx;
	y = cy;
	carvesolid_sphere(
		win, color[0],
		x, y, 0.0,
		r, 0.0, 0.0,
		0.0, 0.0, r
	);

	l = ww*distance_mercury/distance_neptune;
	r = 1024*ww*diameter_mercury/distance_neptune;
	carveline_circle(
		win, 0x404040,
		cx, cy, 0.0,
		l, 0.0, 0.0,
		0.0, 0.0, 1.0
	);
	x = cx + l*cosine(tau*t/period_mercury);
	y = cy + l*sine(tau*t/period_mercury);
	carvesolid_sphere(
		win, color[1],
		x, y, 0.0,
		r, 0.0, 0.0,
		0.0, 0.0, r
	);

	l = ww*distance_venus/distance_neptune;
	r = 1024*ww*diameter_venus/distance_neptune;
	carveline_circle(
		win, 0x404040,
		cx, cy, 0.0,
		l, 0.0, 0.0,
		0.0, 0.0, 1.0
	);
	x = cx + l*cosine(tau*t/period_venus);
	y = cy + l*sine(tau*t/period_venus);
	carvesolid_sphere(
		win, color[2],
		x, y, 0.0,
		r, 0.0, 0.0,
		0.0, 0.0, r
	);

	l = ww*distance_earth/distance_neptune;
	r = 1024*ww*diameter_earth/distance_neptune;
	carveline_circle(
		win, 0x404040,
		cx, cy, 0.0,
		l, 0.0, 0.0,
		0.0, 0.0, 1.0
	);
	x = cx + l*cosine(tau*t/period_earth);
	y = cy + l*sine(tau*t/period_earth);
	carvesolid_sphere(
		win, color[3],
		x, y, 0.0,
		r, 0.0, 0.0,
		0.0, 0.0, r
	);

	l = ww*distance_mars/distance_neptune;
	r = 1024*ww*diameter_mars/distance_neptune;
	carveline_circle(
		win, 0x404040,
		cx, cy, 0.0,
		l, 0.0, 0.0,
		0.0, 0.0, 1.0
	);
	x = cx + l*cosine(tau*t/period_mars);
	y = cy + l*sine(tau*t/period_mars);
	carvesolid_sphere(
		win, color[4],
		x, y, 0.0,
		r, 0.0, 0.0,
		0.0, 0.0, r
	);

	l = ww*distance_jupiter/distance_neptune;
	r = 1024*ww*diameter_jupiter/distance_neptune;
	carveline_circle(
		win, 0x404040,
		cx, cy, 0.0,
		l, 0.0, 0.0,
		0.0, 0.0, 1.0
	);
	x = cx + l*cosine(tau*t/period_jupiter);
	y = cy + l*sine(tau*t/period_jupiter);
	carvesolid_sphere(
		win, color[5],
		x, y, 0.0,
		r, 0.0, 0.0,
		0.0, 0.0, r
	);

	l = ww*distance_saturn/distance_neptune;
	r = 1024*ww*diameter_saturn/distance_neptune;
	carveline_circle(
		win, 0x404040,
		cx, cy, 0.0,
		l, 0.0, 0.0,
		0.0, 0.0, 1.0
	);
	x = cx + l*cosine(tau*t/period_saturn);
	y = cy + l*sine(tau*t/period_saturn);
	carvesolid_sphere(
		win, color[6],
		x, y, 0.0,
		r, 0.0, 0.0,
		0.0, 0.0, r
	);

	l = ww*distance_uranus/distance_neptune;
	r = 1024*ww*diameter_uranus/distance_neptune;
	carveline_circle(
		win, 0x404040,
		cx, cy, 0.0,
		l, 0.0, 0.0,
		0.0, 0.0, 1.0
	);
	x = cx + l*cosine(tau*t/period_uranus);
	y = cy + l*sine(tau*t/period_uranus);
	carvesolid_sphere(
		win, color[7],
		x, y, 0.0,
		r, 0.0, 0.0,
		0.0, 0.0, r
	);

	l = ww*distance_neptune/distance_neptune;
	r = 1024*ww*diameter_neptune/distance_neptune;
	carveline_circle(
		win, 0x404040,
		cx, cy, 0.0,
		l, 0.0, 0.0,
		0.0, 0.0, 1.0
	);
	x = cx + l*cosine(tau*t/period_neptune);
	y = cy + l*sine(tau*t/period_neptune);
	carvesolid_sphere(
		win, color[8],
		x, y, 0.0,
		r, 0.0, 0.0,
		0.0, 0.0, r
	);
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
		"<div id=\"planet\" style=\"width:100%%;height:100px;background-color:#62f4ab;\">"
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
