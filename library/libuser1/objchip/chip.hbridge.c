#include "libuser.h"




static void hbridge_read_pixel(
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
static void hbridge_read_vbo2d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
}
static void hbridge_read_vbo3d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int x,y,z;
	int s,rgb;
	vec3 tc,tr,tf,tu;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	u8 gate[3][2] = {{0,1},{1,0},{0,0}};

	//frame
	carveline_prism4(win, 0xffffff, vc, vr, vf, vu);

	//board
	tc[0] = vc[0] - vu[0];
	tc[1] = vc[1] - vu[1];
	tc[2] = vc[2] - vu[2];
	carvesolid_rect(win, 0x0000ff, tc, vr, vf);
	tc[0] = vc[0] + vu[0];
	tc[1] = vc[1] + vu[1];
	tc[2] = vc[2] + vu[2];
	carvesolid_rect(win, 0xff0000, tc, vr, vf);

	//6 mosfets
	for(z=-1;z<2;z+=2)
	{
		if(z > 0)s = 1;
		else s = -1;
		for(y=-1;y<2;y++)
		{
			//mos
			tc[0] = vc[0] + y*vf[0]/2 + z*vu[0]/2;
			tc[1] = vc[1] + y*vf[1]/2 + z*vu[1]/2;
			tc[2] = vc[2] + y*vf[2]/2 + z*vu[2]/2;
			tr[0] = vr[0]/8;
			tr[1] = vr[1]/8;
			tr[2] = vr[2]/8;
			tf[0] = vf[0]/8;
			tf[1] = vf[1]/8;
			tf[2] = vf[2]/8;
			tu[0] = vu[0]/8;
			tu[1] = vu[1]/8;
			tu[2] = vu[2]/8;
			carvesolid_prism4(win, 0xe0e0e0, tc, tr, tf, tu);

			//g
			tc[0] = vc[0] + y*vf[0]/2 + z*vu[0]/2;
			tc[1] = vc[1] + y*vf[1]/2 + z*vu[1]/2;
			tc[2] = vc[2] + y*vf[2]/2 + z*vu[2]/2;
			tu[0] = tc[0] - vr[0]/2;
			tu[1] = tc[1] - vr[1]/2;
			tu[2] = tc[2] - vr[2]/2;
			if(gate[y+1][(z+1)/2] == 1)rgb = 0xff0000;
			else rgb = 0xffffff;
			carveline(win, rgb, tc, tu);

			//d,s
			tu[0] = tc[0] - s*vu[0]/2;
			tu[1] = tc[1] - s*vu[1]/2;
			tu[2] = tc[2] - s*vu[2]/2;
			carveline(win, 0xffffff, tc, tu);

			//d,s
			tu[0] = tc[0] + s*vu[0]/2;
			tu[1] = tc[1] + s*vu[1]/2;
			tu[2] = tc[2] + s*vu[2]/2;

			if(s>0.0)rgb = 0xff0000;
			else rgb = 0x0000ff;
			carveline(win, rgb, tc, tu);
		}
	}

	for(y=-1;y<2;y++)
	{
		tc[0] = vc[0] + y*vf[0]/2;
		tc[1] = vc[1] + y*vf[1]/2;
		tc[2] = vc[2] + y*vf[2]/2;
		tu[0] = tc[0] + vr[0]/2;
		tu[1] = tc[1] + vr[1]/2;
		tu[2] = tc[2] + vr[2]/2;

		rgb = 0xffffff;
		if((gate[y+1][0] > 0)&&(gate[y+1][1] < 1))rgb = 0x0000ff;
		if((gate[y+1][0] < 1)&&(gate[y+1][1] > 0))rgb = 0xff0000;
		carveline(win, rgb, tc, tu);
	}
}
static void hbridge_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void hbridge_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void hbridge_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void hbridge_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void hbridge_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)hbridge_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)hbridge_read_tui(win, sty, act, pin);
	else if(fmt == _html_)hbridge_read_html(win, sty, act, pin);
	else if(fmt == _json_)hbridge_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)
	{
		if(_2d_ == win->vfmt)hbridge_read_vbo2d(win, sty, act, pin);
		else hbridge_read_vbo3d(win, sty, act, pin);
	}
	else hbridge_read_pixel(win, sty, act, pin);
}




static void hbridge_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void hbridge_post(u8* buf, int len)
{
}
static void hbridge_get(u8* buf, int len)
{
}
static void hbridge_stop(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void hbridge_start(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void hbridge_delete(struct actor* act, u8* buf)
{
}
static void hbridge_create(struct actor* act, u8* buf)
{
}




void hbridge_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('h','b','r','i','d','g','e',0);

	p->oncreate = (void*)hbridge_create;
	p->ondelete = (void*)hbridge_delete;
	p->onstart  = (void*)hbridge_start;
	p->onstop   = (void*)hbridge_stop;
	p->onget    = (void*)hbridge_get;
	p->onpost   = (void*)hbridge_post;
	p->onread   = (void*)hbridge_read;
	p->onwrite  = (void*)hbridge_write;
}
