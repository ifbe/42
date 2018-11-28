#include "libuser.h"




static void rectify_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
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
}
static void rectify_read_vbo2d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
}
static void rectify_read_vbo3d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int x,y,z,rgb;
	u64 time;
	float a,s;
	vec3 tc,tr,tf,tu;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;

	time = timeread() / 1000;
	say("%f\n",time);
	a = time * PI / 1000;
	say("%f\n",a);

	//frame
	tc[0] = vc[0] + vu[0];
	tc[1] = vc[1] + vu[1];
	tc[2] = vc[2] + vu[2];
	carveline_prism4(win, 0xffffff, tc, vr, vf, vu);

	//board
	tr[0] = vr[0]/2;
	tr[1] = vr[1]/2;
	tr[2] = vr[2]/2;
	tf[0] = vf[0]/2;
	tf[1] = vf[1]/2;
	tf[2] = vf[2]/2;
	carvesolid_rect(win, 0x444444, tc, tr, tf);

	//3 inputs
	tr[0] = vr[0]/8;
	tr[1] = vr[1]/8;
	tr[2] = vr[2]/8;
	tf[0] = vf[0]/8;
	tf[1] = vf[1]/8;
	tf[2] = vf[2]/8;
	tu[0] = vu[0]/8;
	tu[1] = vu[1]/8;
	tu[2] = vu[2]/8;
	for(y=-1;y<2;y++)
	{
		s = sine(a + y*PI*2.0/3.0);
		tc[0] = vc[0] - vr[0]/2 + y*vf[0]/2 + (2.0+s)*vu[0]/2;
		tc[1] = vc[1] - vr[1]/2 + y*vf[1]/2 + (2.0+s)*vu[1]/2;
		tc[2] = vc[2] - vr[2]/2 + y*vf[2]/2 + (2.0+s)*vu[2]/2;

		if(s > 0.0)rgb = 0xff0000;
		else rgb = 0xff;
		carvesolid_sphere(win, rgb, tc, tr, tf, tu);
	}
	for(y=-1;y<2;y++)
	{
		tc[0] = vc[0] - vr[0]/2 + y*vf[0]/2 + vu[0]/2;
		tc[1] = vc[1] - vr[1]/2 + y*vf[1]/2 + vu[1]/2;
		tc[2] = vc[2] - vr[2]/2 + y*vf[2]/2 + vu[2]/2;
		tu[0] = tc[0] + vu[0];
		tu[1] = tc[1] + vu[1];
		tu[2] = tc[2] + vu[2];
		carveline(win, 0xffffff, tc, tu);
	}

	//6 diodes
	tr[0] = vu[0]/8;
	tr[1] = vu[1]/8;
	tr[2] = vu[2]/8;
	for(z=1;z<4;z+=2)
	{
		for(y=-1;y<2;y++)
		{
			//+
			tc[0] = vc[0] + (z-2)*vr[0]/16 + y*vf[0]/2 + z*vu[0]/2;
			tc[1] = vc[1] + (z-2)*vr[1]/16 + y*vf[1]/2 + z*vu[1]/2;
			tc[2] = vc[2] + (z-2)*vr[2]/16 + y*vf[2]/2 + z*vu[2]/2;
			tu[0] = vr[0]/4;
			tu[1] = vr[1]/4;
			tu[2] = vr[2]/4;
			carvesolid_cylinder(win, 0xe0e0e0, tc, tr, tu);

			//-
			tc[0] -= (z-2)*vr[0]/3.6;
			tc[1] -= (z-2)*vr[1]/3.6;
			tc[2] -= (z-2)*vr[2]/3.6;
			tu[0] = vr[0]/32;
			tu[1] = vr[1]/32;
			tu[2] = vr[2]/32;
			carvesolid_cylinder(win, 0x202020, tc, tr, tu);

			//wire
			tc[0] = vc[0] - vr[0]/2 + y*vf[0]/2 + z*vu[0]/2;
			tc[1] = vc[1] - vr[1]/2 + y*vf[1]/2 + z*vu[1]/2;
			tc[2] = vc[2] - vr[2]/2 + y*vf[2]/2 + z*vu[2]/2;
			tu[0] = tc[0] + vr[0];
			tu[1] = tc[1] + vr[1];
			tu[2] = tc[2] + vr[2];
			carveline(win, 0xffffff, tc, tu);
		}
	}

	//2 outputs
	for(z=1;z<4;z+=2)
	{
		tc[0] = vc[0] + vr[0]/2 - vf[0]/2 + z*vu[0]/2;
		tc[1] = vc[1] + vr[1]/2 - vf[1]/2 + z*vu[1]/2;
		tc[2] = vc[2] + vr[2]/2 - vf[2]/2 + z*vu[2]/2;
		tu[0] = tc[0] + vf[0];
		tu[1] = tc[1] + vf[1];
		tu[2] = tc[2] + vf[2];
		carveline(win, 0xffffff, tc, tu);
	}
}
static void rectify_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void rectify_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void rectify_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void rectify_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void rectify_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)rectify_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)rectify_read_tui(win, sty, act, pin);
	else if(fmt == _html_)rectify_read_html(win, sty, act, pin);
	else if(fmt == _json_)rectify_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)
	{
		if(_2d_ == win->vfmt)rectify_read_vbo2d(win, sty, act, pin);
		else rectify_read_vbo3d(win, sty, act, pin);
	}
	else rectify_read_pixel(win, sty, act, pin);
}




static void rectify_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void rectify_post(u8* buf, int len)
{
}
static void rectify_get(u8* buf, int len)
{
}
static void rectify_stop(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void rectify_start(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void rectify_delete(struct actor* act, u8* buf)
{
}
static void rectify_create(struct actor* act, u8* buf)
{
}




void rectify_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('r','e','c','t','i','f','y',0);

	p->oncreate = (void*)rectify_create;
	p->ondelete = (void*)rectify_delete;
	p->onstart  = (void*)rectify_start;
	p->onstop   = (void*)rectify_stop;
	p->onget    = (void*)rectify_get;
	p->onpost   = (void*)rectify_post;
	p->onread   = (void*)rectify_read;
	p->onwrite  = (void*)rectify_write;
}
