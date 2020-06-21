#include "libuser.h"




static void geometry_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	int cx = sty->fs.vc[0];
	int cy = sty->fs.vc[1];
	int ww = sty->fs.vr[0];
	int hh = sty->fs.vf[1];
	if(ww > hh)ww = hh;
	if(hh > ww)hh = ww;
	drawsolid_rect(win, 0x808080, cx-ww, cy-hh, cx+ww, cy+hh);
}
static void geometry_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* scn, struct style* geom,
	struct entity* wnd, struct style* area)
{
	vec3 t1,t2;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;
	int dimen = act->iw0;
	int shape = act->iwn;
	//say("%d,%d\n",dimen,shape);

	if('q' == shape){
		gl41point(wnd, 0x808080, vc);
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
			case '2':gl41line(wnd, 0x808080, t1, t2);break;
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
			case '1':gl41point_triangle( wnd, 0x00808080, vc, t1, t2);break;
			case '2':gl41line_triangle(  wnd, 0x00808080, vc, t1, t2);break;
			case '3':gl41solid_triangle( wnd, 0x00808080, vc, t1, t2);break;
			case '4':gl41opaque_triangle(wnd, 0x80808080, vc, t1, t2);break;
		}
		return;
	}

	if('r' == shape){
		switch(dimen){
			case '1':gl41point_rect( wnd, 0x00808080, vc, vr, vf);break;
			case '2':gl41line_rect(  wnd, 0x00808080, vc, vr, vf);break;
			case '3':gl41solid_rect( wnd, 0x00808080, vc, vr, vf);break;
			case '4':gl41opaque_rect(wnd, 0x80808080, vc, vr, vf);break;
		}
		return;
	}

	if('p' == shape){
		switch(dimen){
			//case '1':gl41point_prism4( wnd, 0x808080, vc, vr, vf, vu);break;
			case '2':gl41line_prism4(  wnd, 0x00808080, vc, vr, vf, vu);break;
			case '3':gl41solid_prism4( wnd, 0x00808080, vc, vr, vf, vu);break;
			case '4':gl41opaque_prism4(wnd, 0x80808080, vc, vr, vf, vu);break;
		}
		return;
	}

	if('d' == shape){
		switch(dimen){
			case '1':gl41point_dodecahedron( wnd, 0x00808080, vc, vr, vf, vu);break;
			case '2':gl41line_dodecahedron(  wnd, 0x00808080, vc, vr, vf, vu);break;
			case '3':gl41solid_dodecahedron( wnd, 0x00808080, vc, vr, vf, vu);break;
			case '4':gl41opaque_dodecahedron(wnd, 0x80808080, vc, vr, vf, vu);break;
		}
		return;
	}

	if('i' == shape){
		switch(dimen){
			case '1':gl41point_icosahedron( wnd, 0x00808080, vc, vr, vf, vu);break;
			case '2':gl41line_icosahedron(  wnd, 0x00808080, vc, vr, vf, vu);break;
			case '3':gl41solid_icosahedron( wnd, 0x00808080, vc, vr, vf, vu);break;
			case '4':gl41opaque_icosahedron(wnd, 0x80808080, vc, vr, vf, vu);break;
		}
		return;
	}

	if('c' == shape){
		switch(dimen){
			//case '1':gl41point_cylinder( wnd, 0x808080, vc, vr, vf, vu);break;
			//case '2':gl41line_cylinder(  wnd, 0x808080, vc, vr, vf, vu);break;
			case '3':gl41solid_cylinder( wnd, 0x00808080, vc, vr, vf, vu);break;
			case '4':gl41opaque_cylinder(wnd, 0x80808080, vc, vr, vf, vu);break;
		}
		return;
	}

	if('s' == shape){
		switch(dimen){
			case '1':gl41point_sphere( wnd, 0x00808080, vc, vr, vf, vu);break;
			case '2':gl41line_sphere(  wnd, 0x00808080, vc, vr, vf, vu);break;
			case '3':gl41solid_sphere( wnd, 0x00808080, vc, vr, vf, vu);break;
			case '4':gl41opaque_sphere(wnd, 0x80808080, vc, vr, vf, vu);break;
		}
		return;
	}
}
static void geometry_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void geometry_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void geometry_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void geometry_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void geometry_event(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty,
	struct event* ev, int len)
{
	char c;
	if(_char_ == ev->what)
	{
		c = ev->why;
		if((c>='0') && (c<='9'))act->iw0 = c;
		if((c>='a') && (c<='z'))act->iwn = c;
	}
}




static void geometry_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct style* slot;
	struct entity* scn;struct style* geom;
	struct entity* wnd;struct style* area;

	if(stack&&('v'==key)){
		slot = stack[sp-1].pfoot;
		scn = stack[sp-1].pchip;geom = stack[sp-2].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		geometry_draw_gl41(ent,slot, scn,geom, wnd,area);
	}
}
static void geometry_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void geometry_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void geometry_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void geometry_search(struct entity* act, u8* buf)
{
}
static void geometry_modify(struct entity* act, u8* buf)
{
}
static void geometry_delete(struct entity* act, u8* buf)
{
}
static void geometry_create(struct entity* act, u8* buf)
{
	int dimen = '3';
	int shape = 'p';
	if(buf){
		dimen = buf[0];
		shape = buf[1];
	}
	act->iw0 = dimen;
	act->iwn = shape;
}




void geometry_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex32('g','e','o','m');

	p->oncreate = (void*)geometry_create;
	p->ondelete = (void*)geometry_delete;
	p->onsearch = (void*)geometry_search;
	p->onmodify = (void*)geometry_modify;

	p->onlinkup = (void*)geometry_linkup;
	p->ondiscon = (void*)geometry_discon;
	p->ontaking = (void*)geometry_taking;
	p->ongiving = (void*)geometry_giving;
}
