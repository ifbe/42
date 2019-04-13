#include "libuser.h"




static void geometry_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int cx = sty->vc[0];
	int cy = sty->vc[1];
	int ww = sty->vr[0];
	int hh = sty->vf[1];
	if(ww > hh)ww = hh;
	if(hh > ww)hh = ww;
	drawsolid_rect(win, 0x808080, cx-ww, cy-hh, cx+ww, cy+hh);
}
static void geometry_read_vbo2d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	vec3 tr, tf;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	float n = (win->height) * vf[1] / (win->width) / vr[0];
	if(n < 1.0)
	{
		tr[0] = vr[0] * n;
		tr[1] = vr[1] * n;
		tr[2] = vr[2] * n;
		tf[0] = vf[0];
		tf[1] = vf[1];
		tf[2] = vf[2];
	}
	else
	{
		tr[0] = vr[0];
		tr[1] = vr[1];
		tr[2] = vr[2];
		tf[0] = vf[0] / n;
		tf[1] = vf[1] / n;
		tf[2] = vf[2] / n;
	}
	carveopaque2d_rect(win, 0xffffff, vc, tr, tf);
}
static void geometry_read_vbo3d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	vec3 t1,t2;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	int dimen = act->w0;
	int shape = act->wn;
	//say("%d,%d\n",dimen,shape);

	if('q' == shape){
		carvepoint(win, 0xffffff, vc);
		return;
	}

	if('l' == shape){
		t1[0] = vc[0] - vr[0];
		t1[1] = vc[1] - vr[1];
		t1[2] = vc[2] - vr[2];
		t2[0] = vc[0] + vr[0];
		t2[1] = vc[1] + vr[1];
		t2[2] = vc[2] + vr[2];
		switch(dimen){
			case '1':
			case '2':carveline(win, 0xffffff, t1, t2);break;
		}
		return;
	}

	if('t' == shape){
		t1[0] = vc[0] + vr[0];
		t1[1] = vc[1] + vr[1];
		t1[2] = vc[2] + vr[2];
		t2[0] = vc[0] + vf[0];
		t2[1] = vc[1] + vf[1];
		t2[2] = vc[2] + vf[2];
		switch(dimen){
			case '1':carvepoint_triangle( win, 0xffffff, vc, t1, t2);break;
			case '2':carveline_triangle(  win, 0xffffff, vc, t1, t2);break;
			case '3':carvesolid_triangle( win, 0xffffff, vc, t1, t2);break;
			case '4':carveopaque_triangle(win, 0xffffff, vc, t1, t2);break;
		}
		return;
	}

	if('r' == shape){
		switch(dimen){
			case '1':carvepoint_rect( win, 0xffffff, vc, vr, vf);break;
			case '2':carveline_rect(  win, 0xffffff, vc, vr, vf);break;
			case '3':carvesolid_rect( win, 0xffffff, vc, vr, vf);break;
			case '4':carveopaque_rect(win, 0xffffff, vc, vr, vf);break;
		}
		return;
	}

	if('p' == shape){
		switch(dimen){
			//case '1':carvepoint_prism4( win, 0xffffff, vc, vr, vf, vu);break;
			case '2':carveline_prism4(  win, 0xffffff, vc, vr, vf, vu);break;
			case '3':carvesolid_prism4( win, 0xffffff, vc, vr, vf, vu);break;
			case '4':carveopaque_prism4(win, 0xffffff, vc, vr, vf, vu);break;
		}
		return;
	}

	if('d' == shape){
		switch(dimen){
			case '1':carvepoint_dodecahedron( win, 0xffffff, vc, vr, vf, vu);break;
			case '2':carveline_dodecahedron(  win, 0xffffff, vc, vr, vf, vu);break;
			case '3':carvesolid_dodecahedron( win, 0xffffff, vc, vr, vf, vu);break;
			case '4':carveopaque_dodecahedron(win, 0xffffff, vc, vr, vf, vu);break;
		}
		return;
	}

	if('i' == shape){
		switch(dimen){
			case '1':carvepoint_icosahedron( win, 0xffffff, vc, vr, vf, vu);break;
			case '2':carveline_icosahedron(  win, 0xffffff, vc, vr, vf, vu);break;
			case '3':carvesolid_icosahedron( win, 0xffffff, vc, vr, vf, vu);break;
			case '4':carveopaque_icosahedron(win, 0xffffff, vc, vr, vf, vu);break;
		}
		return;
	}

	if('c' == shape){
		switch(dimen){
			//case '1':carvepoint_cylinder( win, 0xffffff, vc, vr, vf, vu);break;
			//case '2':carveline_cylinder(  win, 0xffffff, vc, vr, vf, vu);break;
			case '3':carvesolid_cylinder( win, 0xffffff, vc, vr, vf, vu);break;
			case '4':carveopaque_cylinder(win, 0xffffff, vc, vr, vf, vu);break;
		}
		return;
	}

	if('s' == shape){
		switch(dimen){
			case '1':carvepoint_sphere( win, 0xffffff, vc, vr, vf, vu);break;
			case '2':carveline_sphere(  win, 0xffffff, vc, vr, vf, vu);break;
			case '3':carvesolid_sphere( win, 0xffffff, vc, vr, vf, vu);break;
			case '4':carveopaque_sphere(win, 0xffffff, vc, vr, vf, vu);break;
		}
		return;
	}
}
static void geometry_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void geometry_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void geometry_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void geometry_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void geometry_sread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)geometry_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)geometry_read_tui(win, sty, act, pin);
	else if(fmt == _html_)geometry_read_html(win, sty, act, pin);
	else if(fmt == _json_)geometry_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)
	{
		if(_2d_ == win->vfmt)geometry_read_vbo2d(win, sty, act, pin);
		else geometry_read_vbo3d(win, sty, act, pin);
	}
	else geometry_read_pixel(win, sty, act, pin);
}
static void geometry_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	char c;
	if(_char_ == ev->what)
	{
		c = ev->why;
		if((c>='0') && (c<='9'))act->w0 = c;
		if((c>='a') && (c<='z'))act->wn = c;
	}
}
static void geometry_cread(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void geometry_cwrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	u8* buf, int len)
{
}
static void geometry_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
}
static void geometry_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
	struct arena* root, struct style* rf)
{
}
static void geometry_delete(struct actor* act, u8* buf)
{
}
static void geometry_create(struct actor* act, u8* buf)
{
	int dimen = '3';
	int shape = 's';
	if(buf){
		dimen = buf[0];
		shape = buf[1];
	}
	act->w0 = dimen;
	act->wn = shape;
}




void geometry_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex32('g','e','o','m');

	p->oncreate = (void*)geometry_create;
	p->ondelete = (void*)geometry_delete;
	p->onstart  = (void*)geometry_start;
	p->onstop   = (void*)geometry_stop;
	p->oncread  = (void*)geometry_cread;
	p->oncwrite = (void*)geometry_cwrite;
	p->onsread  = (void*)geometry_sread;
	p->onswrite = (void*)geometry_swrite;
}
