#include "libuser.h"
double arctan2(double,double);
double cosine(double);
double sine(double);



static int px=0;
static int py=0;
static u8 buffer[16];




void doodle_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	float c,s,f;
	int x0,y0,x1,y1;
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
static void doodle_draw_vbo(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	float a,c,s;
	vec3 tc, tr, tf, tu, f;
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;
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
static void doodle_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void doodle_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
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
static void doodle_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void doodle_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	say("doodle(%x,%x,%x)\n", act, pin, win, sty);
}
static void doodle_draw(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)doodle_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)doodle_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)doodle_draw_html(act, pin, win, sty);
	else if(fmt == _json_)doodle_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)doodle_draw_vbo(act, pin, win, sty);
	else doodle_draw_pixel(act, pin, win, sty);
}
static void doodle_event(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty,
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




static void doodle_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	//if 'draw' == self.foot
	struct entity* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct entity* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	//doodle_draw(act, pin, win, sty);
}
static void doodle_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	//if 'ev i' == self.foot
	struct entity* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct entity* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct event* ev = (void*)buf;
	//doodle_event(act, pin, win, sty, ev, 0);
}
static void doodle_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void doodle_start(struct halfrel* self, struct halfrel* peer)
{
}




static void doodle_search(struct entity* act)
{
}
static void doodle_modify(struct entity* act)
{
}
static void doodle_delete(struct entity* act)
{
}
static void doodle_create(struct entity* act)
{
	if(0 == act)return;
	act->buf = ((void*)act) + 0x100;
}




void doodle_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('d', 'o', 'o', 'd', 'l', 'e', 0, 0);

	p->oncreate = (void*)doodle_create;
	p->ondelete = (void*)doodle_delete;
	p->onsearch = (void*)doodle_search;
	p->onmodify = (void*)doodle_modify;

	p->onstart = (void*)doodle_start;
	p->onstop  = (void*)doodle_stop;
	p->onread  = (void*)doodle_read;
	p->onwrite = (void*)doodle_write;
}
