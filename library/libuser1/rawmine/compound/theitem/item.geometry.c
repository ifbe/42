#include "libuser.h"




static void geometry_draw_pixel(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	int cx = sty->f.vc[0];
	int cy = sty->f.vc[1];
	int ww = sty->f.vr[0];
	int hh = sty->f.vf[1];
	if(ww > hh)ww = hh;
	if(hh > ww)hh = ww;
	drawsolid_rect(win, 0x808080, cx-ww, cy-hh, cx+ww, cy+hh);
}
static void geometry_draw_vbo2d(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	vec3 tr, tf;
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;
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
	carveopaque2d_rect(win, 0x7fffffff, vc, tr, tf);
}
static void geometry_draw_vbo3d(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	vec3 t1,t2;
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;
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
			case '1':carvepoint_triangle( win, 0x00ffffff, vc, t1, t2);break;
			case '2':carveline_triangle(  win, 0x00ffffff, vc, t1, t2);break;
			case '3':carvesolid_triangle( win, 0x00ffffff, vc, t1, t2);break;
			case '4':carveopaque_triangle(win, 0x7fffffff, vc, t1, t2);break;
		}
		return;
	}

	if('r' == shape){
		switch(dimen){
			case '1':carvepoint_rect( win, 0x00ffffff, vc, vr, vf);break;
			case '2':carveline_rect(  win, 0x00ffffff, vc, vr, vf);break;
			case '3':carvesolid_rect( win, 0x00ffffff, vc, vr, vf);break;
			case '4':carveopaque_rect(win, 0x7fffffff, vc, vr, vf);break;
		}
		return;
	}

	if('p' == shape){
		switch(dimen){
			//case '1':carvepoint_prism4( win, 0xffffff, vc, vr, vf, vu);break;
			case '2':carveline_prism4(  win, 0x00ffffff, vc, vr, vf, vu);break;
			case '3':carvesolid_prism4( win, 0x00ffffff, vc, vr, vf, vu);break;
			case '4':carveopaque_prism4(win, 0x7fffffff, vc, vr, vf, vu);break;
		}
		return;
	}

	if('d' == shape){
		switch(dimen){
			case '1':carvepoint_dodecahedron( win, 0x00ffffff, vc, vr, vf, vu);break;
			case '2':carveline_dodecahedron(  win, 0x00ffffff, vc, vr, vf, vu);break;
			case '3':carvesolid_dodecahedron( win, 0x00ffffff, vc, vr, vf, vu);break;
			case '4':carveopaque_dodecahedron(win, 0x7fffffff, vc, vr, vf, vu);break;
		}
		return;
	}

	if('i' == shape){
		switch(dimen){
			case '1':carvepoint_icosahedron( win, 0x00ffffff, vc, vr, vf, vu);break;
			case '2':carveline_icosahedron(  win, 0x00ffffff, vc, vr, vf, vu);break;
			case '3':carvesolid_icosahedron( win, 0x00ffffff, vc, vr, vf, vu);break;
			case '4':carveopaque_icosahedron(win, 0x7fffffff, vc, vr, vf, vu);break;
		}
		return;
	}

	if('c' == shape){
		switch(dimen){
			//case '1':carvepoint_cylinder( win, 0xffffff, vc, vr, vf, vu);break;
			//case '2':carveline_cylinder(  win, 0xffffff, vc, vr, vf, vu);break;
			case '3':carvesolid_cylinder( win, 0x00ffffff, vc, vr, vf, vu);break;
			case '4':carveopaque_cylinder(win, 0x7fffffff, vc, vr, vf, vu);break;
		}
		return;
	}

	if('s' == shape){
		switch(dimen){
			case '1':carvepoint_sphere( win, 0x00ffffff, vc, vr, vf, vu);break;
			case '2':carveline_sphere(  win, 0x00ffffff, vc, vr, vf, vu);break;
			case '3':carvesolid_sphere( win, 0x00ffffff, vc, vr, vf, vu);break;
			case '4':carveopaque_sphere(win, 0x7fffffff, vc, vr, vf, vu);break;
		}
		return;
	}
}
static void geometry_draw_json(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void geometry_draw_html(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void geometry_draw_tui(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void geometry_draw_cli(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void geometry_draw(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)geometry_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)geometry_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)geometry_draw_html(act, pin, win, sty);
	else if(fmt == _json_)geometry_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)
	{
		//if(_2d_ == win->vfmt)geometry_draw_vbo2d(act, pin, win, sty);
		//else geometry_draw_vbo3d(act, pin, win, sty);
	}
	else geometry_draw_pixel(act, pin, win, sty);
}
static void geometry_event(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty,
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




static void geometry_read(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	//geometry_draw(act, pin, win, sty);
}
static void geometry_write(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'ev i' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct event* ev = (void*)buf;
	//geometry_event(act, pin, win, sty, ev, 0);
}
static void geometry_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void geometry_start(struct halfrel* self, struct halfrel* peer)
{
}




static void geometry_search(struct actor* act, u8* buf)
{
}
static void geometry_modify(struct actor* act, u8* buf)
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
	p->onsearch = (void*)geometry_search;
	p->onmodify = (void*)geometry_modify;

	p->onstart = (void*)geometry_start;
	p->onstop  = (void*)geometry_stop;
	p->onread  = (void*)geometry_read;
	p->onwrite = (void*)geometry_write;
}
