#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151




static void mobius_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void mobius_draw_gl41(
	_obj* act, struct style* slot,
	_obj* scn, struct style* geom,
	_obj* wnd, struct style* area)
{
	int j,rgb;
	float a,c,s;
	float x,y,z,n;
	vec3 tc,tr,tf,tu;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;

	tc[0] = vc[0] + vu[0];
	tc[1] = vc[1] + vu[1];
	tc[2] = vc[2] + vu[2];
	tr[0] = vr[0]*0.75;
	tr[1] = vr[1]*0.75;
	tr[2] = vr[2]*0.75;
	tf[0] = vf[0]*0.75;
	tf[1] = vf[1]*0.75;
	tf[2] = vf[2]*0.75;
	gl41line_circle(wnd, 0xff0000, tc, tr, tf);

	act->whdf.ix0 = (timeread_us()/10000) % 360;
	for(j=0;j<180;j++)
	{
		a = j*PI/90;
		c = getcos(a);
		s = getsin(a);

		tr[0] = vr[0]*c + vf[0]*s;
		tr[1] = vr[1]*c + vf[1]*s;
		tr[2] = vr[2]*c + vf[2]*s;

		tc[0] = vc[0] + tr[0]*0.75 + vu[0];
		tc[1] = vc[1] + tr[1]*0.75 + vu[1];
		tc[2] = vc[2] + tr[2]*0.75 + vu[2];

		//cos' = -sin, sin' = cos
		x = -s*vr[0] + c*vf[0];
		y = -s*vr[1] + c*vf[1];
		z = -s*vr[2] + c*vf[2];
		n = 1.0/squareroot(x*x + y*y + z*z);
		tf[0] = x*n;
		tf[1] = y*n;
		tf[2] = z*n;

		//
		c = getcos(a/2);
		s = getsin(a/2);
		tr[0] = (tr[0]*c + vu[0]*s)*0.25;
		tr[1] = (tr[1]*c + vu[1]*s)*0.25;
		tr[2] = (tr[2]*c + vu[2]*s)*0.25;

		//tu = cross(tr, tf)
		x = tr[1] * tf[2] - tr[2] * tf[1];
		y = tr[2] * tf[0] - tr[0] * tf[2];
		z = tr[0] * tf[1] - tr[1] * tf[0];
		n = 1.0 / squareroot(x*x + y*y + z*z);

		tu[0] = x * n;
		tu[1] = y * n;
		tu[2] = z * n;

		if(j == act->whdf.ix0)rgb = 0xff0000;
		else rgb = 0x808080;
		gl41solid_prism4(wnd, rgb, tc, tr, tf, tu);
	}
}
static void mobius_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void mobius_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void mobius_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void mobius_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void mobius_wrl_cam_wnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	mobius_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void mobius_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	//foot defined behavior
	switch(stack[sp-1].foottype){
	}

	//caller defined behavior
	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(caller->hfmt){
	case _rgba_:
		break;
	case _gl41list_:
		break;
	default:
		mobius_wrl_cam_wnd(ent,foot, stack,sp);
		break;
	}
}
static void mobius_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void mobius_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void mobius_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void mobius_search(_obj* act)
{
}
static void mobius_modify(_obj* act)
{
}
static void mobius_delete(_obj* act)
{
	if(0 == act)return;
	//if(_copy_ == act->type)memoryfree(act->buf);
}
static void mobius_create(_obj* act)
{
	if(0 == act)return;
	//if(_orig_ == act->type)act->buf = buffer;
	//if(_copy_ == act->type)act->buf = memoryalloc(256, 0);
}




void mobius_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('m', 'o', 'b', 'i', 'u', 's', 0, 0);

	p->oncreate = (void*)mobius_create;
	p->ondelete = (void*)mobius_delete;
	p->onreader = (void*)mobius_search;
	p->onwriter = (void*)mobius_modify;

	p->onattach = (void*)mobius_attach;
	p->ondetach = (void*)mobius_detach;
	p->ontaking = (void*)mobius_taking;
	p->ongiving = (void*)mobius_giving;
}
