#include "libuser.h"
double arctanyx(double,double);
double getcos(double);
double getsin(double);



static int px=0;
static int py=0;
static u8 buffer[16];




void doodle_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	float c,s,f;
	int x0,y0,x1,y1;
	int cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->fs.vc[0];
		cy = sty->fs.vc[1];
		ww = sty->fs.vr[0];
		hh = sty->fs.vf[1];
	}
	else
	{
		cx = win->whdf.width/2;
		cy = win->whdf.height/2;
		ww = win->whdf.width/2;
		hh = win->whdf.height/2;
	}

	//circle
	drawsolid_circle(win, 0x00ffff, cx, cy, ww);
	drawsolid_circle(win, 0x404040, cx+ww/2, cy, ww/2);
	drawsolid_circle(win, 0x404040, cx-ww/2, cy, ww/2);

	f = arctanyx(py-cy, px-cx+(ww/2));
	c = getcos(f);
	s = getsin(f);
	x0 = (int)(c*ww/4) + (cx-ww/2);
	y0 = (int)(s*ww/4) + cy;

	f = arctanyx(py-cy, px-cx-(ww/2));
	c = getcos(f);
	s = getsin(f);
	x1 = (int)(c*ww/4) + (cx+ww/2);
	y1 = (int)(s*ww/4) + cy;

	drawsolid_circle(win, 0xff0000, x0, y0, ww/4);
	drawsolid_circle(win, 0xff0000, x1, y1, ww/4);

	drawline(win, 0xffffff, x0, y0, px, py);
	drawline(win, 0xffffff, x1, y1, px, py);

	drawline_bezier(win, 0xffffff, x0, y0, x1, y1, px, py);
}
static void doodle_draw_gl41(
	_obj* act, struct style* slot,
	_obj* wrl, struct style* geom,
	_obj* wnd, struct style* area)
{
	float a,c,s;
	vec3 tc, tr, tf, tu, f;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;
	gl41solid_circle(wnd, 0x00ffff, vc, vr, vf);

	tr[0] = vr[0]/2;
	tr[1] = vr[1]/2;
	tr[2] = vr[2]/2;
	tf[0] = vf[0]/2;
	tf[1] = vf[1]/2;
	tf[2] = vf[2]/2;

	tc[0] = vc[0]-vr[0]/2+vu[0]/8;
	tc[1] = vc[1]-vr[1]/2+vu[1]/8;
	tc[2] = vc[2]-vr[2]/2+vu[2]/8,
	gl41solid_circle(wnd, 0x404040, tc, tr, tf);

	tc[0] = vc[0]+vr[0]/2+vu[0]/8;
	tc[1] = vc[1]+vr[1]/2+vu[1]/8;
	tc[2] = vc[2]+vr[2]/2+vu[2]/8;
	gl41solid_circle(wnd, 0x404040, tc, tr, tf);

	tr[0] = vr[0]/4;
	tr[1] = vr[1]/4;
	tr[2] = vr[2]/4;
	tf[0] = vf[0]/4;
	tf[1] = vf[1]/4;
	tf[2] = vf[2]/4;

	a = arctanyx(py-vc[1]+vr[1], px-vc[0]+vr[0]);
	c = getcos(a);
	s = getsin(a);
	tc[0] = vc[0]-vr[0]/2+(vr[0]*c+vf[0]*s)/4+vu[0]/4;
	tc[1] = vc[1]-vr[1]/2+(vr[1]*c+vf[1]*s)/4+vu[1]/4;
	tc[2] = vc[2]-vr[2]/2+(vr[2]*c+vf[2]*s)/4+vu[2]/4;
	gl41solid_circle(wnd, 0xff0000, tc, tr, tf);

	tu[0] = px;
	tu[1] = py;
	tu[2] = 0.0;
	gl41line(wnd, 0xffffff, tc, tu);

	a = arctanyx(py-vc[1]-vr[0], px-vc[0]-vr[1]);
	c = getcos(a);
	s = getsin(a);
	tc[0] = vc[0]+vr[0]/2+(vr[0]*c+vf[0]*s)/4+vu[0]/4;
	tc[1] = vc[1]+vr[1]/2+(vr[1]*c+vf[1]*s)/4+vu[1]/4;
	tc[2] = vc[2]+vr[2]/2+(vr[2]*c+vf[2]*s)/4+vu[2]/4;
	gl41solid_circle(wnd, 0xff0000, tc, tr, tf);

	tu[0] = px;
	tu[1] = py;
	tu[2] = 0.0;
	gl41line(wnd, 0xffffff, tc, tu);
}
static void doodle_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void doodle_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void doodle_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void doodle_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	say("doodle(%x,%x,%x)\n", act, pin, win, sty);
}
static void doodle_event(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty,
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




static void doodle_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void doodle_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void doodle_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void doodle_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void doodle_search(_obj* act)
{
}
static void doodle_modify(_obj* act)
{
}
static void doodle_delete(_obj* act)
{
}
static void doodle_create(_obj* act)
{
}




void doodle_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('d', 'o', 'o', 'd', 'l', 'e', 0, 0);

	p->oncreate = (void*)doodle_create;
	p->ondelete = (void*)doodle_delete;
	p->onreader = (void*)doodle_search;
	p->onwriter = (void*)doodle_modify;

	p->onattach = (void*)doodle_attach;
	p->ondetach = (void*)doodle_detach;
	p->ontaking = (void*)doodle_taking;
	p->ongiving = (void*)doodle_giving;
}
