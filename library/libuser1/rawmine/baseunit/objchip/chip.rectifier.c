#include "libuser.h"
void rectify_chosen(float* v, int* t)
{
	int j;
	float hi,lo;

	//highest
	t[0] = t[1] = 0;
	hi = v[0];
	lo = v[0];
	for(j=1;j<3;j++)
	{
		if(hi < v[j])
		{
			t[0] = j;
			hi = v[j];
		}
		if(lo > v[j])
		{
			t[1] = j;
			lo = v[j];
		}
	}
}




static void rectify_draw_pixel(
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
}/*
static void rectify_draw_vbo2d(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;
}*/
static void rectify_draw_vbo3d(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	u64 time;
	int chosen[2];
	int x,y,z,rgb;
	float a,s[3];
	vec3 tc,tr,tf,tu;
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;

	time = timeread() / 1000;
	a = time * PI / 1000;
	s[0] = sine(a - PI*2.0/3.0);
	s[1] = sine(a);
	s[2] = sine(a + PI*2.0/3.0);
	rectify_chosen(s, chosen);

	//frame
	carveline_prism4(win, 0xffffff, vc, vr, vf, vu);

	//board
	tr[0] = vr[0]/2;
	tr[1] = vr[1]/2;
	tr[2] = vr[2]/2;
	tf[0] = vf[0]/2;
	tf[1] = vf[1]/2;
	tf[2] = vf[2]/2;
	tu[0] = vu[0]/16;
	tu[1] = vu[1]/16;
	tu[2] = vu[2]/16;
	carvesolid_prism4(win, 0x444444, vc, tr, tf, tu);

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
		a = s[y+1];
		if(a > 0.0)rgb = 0xff0000;
		else rgb = 0xff;

		tc[0] = vc[0] - vr[0]/2 + y*vf[0]/2 + a*vu[0]/2;
		tc[1] = vc[1] - vr[1]/2 + y*vf[1]/2 + a*vu[1]/2;
		tc[2] = vc[2] - vr[2]/2 + y*vf[2]/2 + a*vu[2]/2;
		carvesolid_sphere(win, rgb, tc, tr, tf, tu);
	}
	for(y=-1;y<2;y++)
	{
		tc[0] = vc[0] - vr[0]/2 + y*vf[0]/2 - vu[0]/2;
		tc[1] = vc[1] - vr[1]/2 + y*vf[1]/2 - vu[1]/2;
		tc[2] = vc[2] - vr[2]/2 + y*vf[2]/2 - vu[2]/2;
		tu[0] = tc[0] + vu[0];
		tu[1] = tc[1] + vu[1];
		tu[2] = tc[2] + vu[2];
		carveline(win, 0xffffff, tc, tu);
	}

	//6 diodes
	tr[0] = vu[0]/8;
	tr[1] = vu[1]/8;
	tr[2] = vu[2]/8;
	tf[0] = vf[0]/8;
	tf[1] = vf[1]/8;
	tf[2] = vf[2]/8;
	for(z=-1;z<2;z+=2)
	{
		for(y=-1;y<2;y++)
		{
			//-
			tc[0] = vc[0] + z*vr[0]/16 + y*vf[0]/2 + z*vu[0]/2;
			tc[1] = vc[1] + z*vr[1]/16 + y*vf[1]/2 + z*vu[1]/2;
			tc[2] = vc[2] + z*vr[2]/16 + y*vf[2]/2 + z*vu[2]/2;
			tu[0] = vr[0] / 32;
			tu[1] = vr[1] / 32;
			tu[2] = vr[2] / 32;
			carvesolid_cylinder(win, 0x808080, tc, tr, tf, tu);

			//+
			tc[0] -= z*vr[0]/3.6;
			tc[1] -= z*vr[1]/3.6;
			tc[2] -= z*vr[2]/3.6;
			tu[0] = vr[0] / 4;
			tu[1] = vr[1] / 4;
			tu[2] = vr[2] / 4;
			carvesolid_cylinder(win, 0x404040, tc, tr, tf, tu);

			a = s[y+1];
			rgb = 0xffffff;
			if(z>0)		//if highest
			{
				if(y+1 == chosen[0])rgb = 0xff0000;
			}
			else		//if lowest
			{
				if(y+1 == chosen[1])rgb = 0xff;
			}

			//wire
			tc[0] = vc[0] - vr[0]/2 + y*vf[0]/2 + z*vu[0]/2;
			tc[1] = vc[1] - vr[1]/2 + y*vf[1]/2 + z*vu[1]/2;
			tc[2] = vc[2] - vr[2]/2 + y*vf[2]/2 + z*vu[2]/2;
			tu[0] = tc[0] + vr[0];
			tu[1] = tc[1] + vr[1];
			tu[2] = tc[2] + vr[2];
			carveline(win, rgb, tc, tu);
		}
	}

	//2 outputs
	tr[0] = vr[0]/8;
	tr[1] = vr[1]/8;
	tr[2] = vr[2]/8;
	tf[0] = vf[0]/8;
	tf[1] = vf[1]/8;
	tf[2] = vf[2]/8;
	tu[0] = vu[0]/8;
	tu[1] = vu[1]/8;
	tu[2] = vu[2]/8;
	tc[0] = vc[0] + vr[0]/2 + s[chosen[0]]*vu[0]/2;
	tc[1] = vc[1] + vr[1]/2 + s[chosen[0]]*vu[1]/2;
	tc[2] = vc[2] + vr[2]/2 + s[chosen[0]]*vu[2]/2;
	carvesolid_sphere(win, 0xff0000, tc, tr, tf, tu);
	tc[0] = vc[0] + vr[0]/2 + s[chosen[1]]*vu[0]/2;
	tc[1] = vc[1] + vr[1]/2 + s[chosen[1]]*vu[1]/2;
	tc[2] = vc[2] + vr[2]/2 + s[chosen[1]]*vu[2]/2;
	carvesolid_sphere(win, 0x0000ff, tc, tr, tf, tu);

	for(z=-1;z<2;z+=2)
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
static void rectify_draw_json(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void rectify_draw_html(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void rectify_draw_tui(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void rectify_draw_cli(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void rectify_draw(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)rectify_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)rectify_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)rectify_draw_html(act, pin, win, sty);
	else if(fmt == _json_)rectify_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)
	{
		//if(_2d_ == win->vfmt)rectify_draw_vbo2d(act, pin, win, sty);
		//else rectify_draw_vbo3d(act, pin, win, sty);
	}
	else rectify_draw_pixel(act, pin, win, sty);
}




static void rectify_read(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	//rectify_draw(act, pin, win, sty);
}
static void rectify_write(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void rectify_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void rectify_start(struct halfrel* self, struct halfrel* peer)
{
}




static void rectify_search(struct actor* act, u8* buf)
{
}
static void rectify_modify(struct actor* act, u8* buf)
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
	p->fmt = hex64('r','e','c','t','i','f','y',0);

	p->oncreate = (void*)rectify_create;
	p->ondelete = (void*)rectify_delete;
	p->onsearch = (void*)rectify_search;
	p->onmodify = (void*)rectify_modify;

	p->onstart = (void*)rectify_start;
	p->onstop  = (void*)rectify_stop;
	p->onread  = (void*)rectify_read;
	p->onwrite = (void*)rectify_write;
}
