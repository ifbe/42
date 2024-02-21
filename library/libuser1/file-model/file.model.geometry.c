#include "libuser.h"




static void geometry_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
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
	_obj* act, struct style* slot,
	_obj* scn, struct style* geom,
	_obj* wnd, struct style* area)
{
	vec3 t1,t2;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;
	int dimen = act->whdf.iw0;
	int shape = act->whdf.iwn;
	u32 color = act->whdf.ix0;
	logtoall("dim=%c,shape=%c,color=%x\n",dimen,shape,color);

	if('q' == shape){
		gl41point(wnd, color, vc);
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
			case '2':gl41line(wnd, color, t1, t2);break;
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
			case '1':gl41point_triangle( wnd, color, vc, t1, t2);break;
			case '2':gl41line_triangle(  wnd, color, vc, t1, t2);break;
			case '3':gl41solid_triangle( wnd, color, vc, t1, t2);break;
			case '4':gl41opaque_triangle(wnd, color, vc, t1, t2);break;
		}
		return;
	}

	if('r' == shape){
		switch(dimen){
			case '1':gl41point_rect( wnd, color, vc, vr, vf);break;
			case '2':gl41line_rect(  wnd, color, vc, vr, vf);break;
			case '3':gl41solid_rect( wnd, color, vc, vr, vf);break;
			case '4':gl41opaque_rect(wnd, color, vc, vr, vf);break;
		}
		return;
	}

	if('p' == shape){
		switch(dimen){
			//case '1':gl41point_prism4( wnd, color, vc, vr, vf, vu);break;
			case '2':gl41line_prism4(  wnd, color, vc, vr, vf, vu);break;
			case '3':gl41solid_prism4( wnd, color, vc, vr, vf, vu);break;
			case '4':gl41opaque_prism4(wnd, color, vc, vr, vf, vu);break;
		}
		return;
	}

	if('d' == shape){
		switch(dimen){
			case '1':gl41point_dodecahedron( wnd, color, vc, vr, vf, vu);break;
			case '2':gl41line_dodecahedron(  wnd, color, vc, vr, vf, vu);break;
			case '3':gl41solid_dodecahedron( wnd, color, vc, vr, vf, vu);break;
			case '4':gl41opaque_dodecahedron(wnd, color, vc, vr, vf, vu);break;
		}
		return;
	}

	if('i' == shape){
		switch(dimen){
			case '1':gl41point_icosahedron( wnd, color, vc, vr, vf, vu);break;
			case '2':gl41line_icosahedron(  wnd, color, vc, vr, vf, vu);break;
			case '3':gl41solid_icosahedron( wnd, color, vc, vr, vf, vu);break;
			case '4':gl41opaque_icosahedron(wnd, color, vc, vr, vf, vu);break;
		}
		return;
	}

	if('c' == shape){
		switch(dimen){
			//case '1':gl41point_cylinder( wnd, color, vc, vr, vf, vu);break;
			//case '2':gl41line_cylinder(  wnd, color, vc, vr, vf, vu);break;
			case '3':gl41solid_cylinder( wnd, color, vc, vr, vf, vu);break;
			case '4':gl41opaque_cylinder(wnd, color, vc, vr, vf, vu);break;
		}
		return;
	}

	if('s' == shape){
		switch(dimen){
			case '1':gl41point_sphere( wnd, color, vc, vr, vf, vu);break;
			case '2':gl41line_sphere(  wnd, color, vc, vr, vf, vu);break;
			case '3':gl41solid_sphere( wnd, color, vc, vr, vf, vu);break;
			case '4':gl41opaque_sphere(wnd, color, vc, vr, vf, vu);break;
		}
		return;
	}
}
static void geometry_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void geometry_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void geometry_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void geometry_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void geometry_event(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty,
	struct event* ev, int len)
{
	char c;
	if(_char_ == ev->what)
	{
		c = ev->why;
		if((c>='0') && (c<='9'))act->whdf.iw0 = c;
		if((c>='a') && (c<='z'))act->whdf.iwn = c;
	}
}




static void geometry_read_bywnd(_obj* ent,void* slot, _obj* wnd,void* area)
{
	switch(wnd->vfmt){
	case _rgba8888_:
		break;
	case _gl41list_:
		break;
	}
}
static void geometry_read_byworld_bycam_bywnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* scn;struct style* geom;
	_obj* wnd;struct style* area;

	scn = stack[sp-1].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	geometry_draw_gl41(ent,slot, scn,geom, wnd,area);
}




static void geometry_taking(_obj* ent,void* slot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	//foot defined behavior
	switch(stack[sp-1].foottype){
	}

	//caller defined behavior
	switch(caller->type){
	case _wnd_:
		geometry_read_bywnd(ent,slot, caller,area);
		break;
	default:
		geometry_read_byworld_bycam_bywnd(ent,slot, stack,sp);
	}
}
static void geometry_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void geometry_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void geometry_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void geometry_search(_obj* act, u8* buf)
{
}
static void geometry_modify(_obj* act, u8* buf)
{
}
static void geometry_delete(_obj* act, u8* buf)
{
}
static void geometry_create(_obj* act, u8* buf, int argc, u8** argv)
{
	int dimen = '4';
	int shape = 's';
	u32 color = 0x80808080;

	int j;
	for(j=0;j<argc;j++){
		if(0 == ncmp(argv[j], "color:", 6)){
			hexstr2u32(argv[j]+6, (void*)&color);
		}
	}

	act->whdf.iw0 = dimen;
	act->whdf.iwn = shape;
	act->whdf.ix0 = color;
}




void geometry_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex32('g','e','o','m');

	p->oncreate = (void*)geometry_create;
	p->ondelete = (void*)geometry_delete;
	p->onreader = (void*)geometry_search;
	p->onwriter = (void*)geometry_modify;

	p->onattach = (void*)geometry_attach;
	p->ondetach = (void*)geometry_detach;
	p->ontaking = (void*)geometry_taking;
	p->ongiving = (void*)geometry_giving;
}
