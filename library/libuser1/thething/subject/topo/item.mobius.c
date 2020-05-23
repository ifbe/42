#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151




static void mobius_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void mobius_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* scn, struct style* geom,
	struct entity* wnd, struct style* area)
{
	int j,rgb;
	float a,c,s;
	float x,y,z,n;
	vec3 tc,tr,tf,tu;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vu = geom->f.vt;

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

	act->ix0 = (timeread()/10000) % 360;
	for(j=0;j<180;j++)
	{
		a = j*PI/90;
		c = cosine(a);
		s = sine(a);

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
		c = cosine(a/2);
		s = sine(a/2);
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

		if(j == act->ix0)rgb = 0xff0000;
		else rgb = 0x808080;
		gl41solid_prism4(wnd, rgb, tc, tr, tf, tu);
	}
}
static void mobius_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void mobius_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void mobius_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void mobius_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void mobius_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	if(stack&&('v' == key)){
		slot = stack[sp-1].pfoot;
		wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		mobius_draw_gl41(ent,slot, wor,geom, wnd,area);
	}
}
static void mobius_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void mobius_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void mobius_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void mobius_search(struct entity* act)
{
}
static void mobius_modify(struct entity* act)
{
}
static void mobius_delete(struct entity* act)
{
	if(0 == act)return;
	//if(_copy_ == act->type)memorydelete(act->buf);
}
static void mobius_create(struct entity* act)
{
	if(0 == act)return;
	//if(_orig_ == act->type)act->buf = buffer;
	//if(_copy_ == act->type)act->buf = memorycreate(256, 0);
}




void mobius_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('m', 'o', 'b', 'i', 'u', 's', 0, 0);

	p->oncreate = (void*)mobius_create;
	p->ondelete = (void*)mobius_delete;
	p->onsearch = (void*)mobius_search;
	p->onmodify = (void*)mobius_modify;

	p->onlinkup = (void*)mobius_linkup;
	p->ondiscon = (void*)mobius_discon;
	p->ontaking = (void*)mobius_taking;
	p->ongiving = (void*)mobius_giving;
}
