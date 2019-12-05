#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151




static void robodog_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void robodog_draw_vbo(
	struct entity* act, struct style* slot,
	struct entity* scn, struct style* geom,
	struct entity* ctx, struct style* area)
{
	vec3 tc,tr,tf,tt;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vt = geom->f.vt;

	//body
	tc[0] = vc[0] + vt[0];
	tc[1] = vc[1] + vt[1];
	tc[2] = vc[2] + vt[2];
	tr[0] = vr[0]*0.875;
	tr[1] = vr[1]*0.875;
	tr[2] = vr[2]*0.875;
	tt[0] = vt[0]/16;
	tt[1] = vt[1]/16;
	tt[2] = vt[2]/16;
	carvesolid_prism4(ctx, 0x202020, tc, tr, vf, tt);


	//
	tr[0] = vr[0]/16;
	tr[1] = vr[1]/16;
	tr[2] = vr[2]/16;
	tf[0] = vf[0]/16;
	tf[1] = vf[1]/16;
	tf[2] = vf[2]/16;

	//l,b
	tc[0] = vc[0] -vr[0]*0.875 -vf[0] +vt[0];
	tc[1] = vc[1] -vr[1]*0.875 -vf[1] +vt[1];
	tc[2] = vc[2] -vr[2]*0.875 -vf[2] +vt[2];
	carvesolid_cylinder(ctx, 0x800000, tc, tr, tt, tf);
	tc[0] = vc[0] -vr[0]+tr[0] -vf[0] +vt[0];
	tc[1] = vc[1] -vr[1]+tr[1] -vf[1] +vt[1];
	tc[2] = vc[2] -vr[2]+tr[2] -vf[2] +vt[2];
	carvesolid_prism4(ctx, 0x800000, tc, tr, tf, tt);

	//r,b
	tc[0] = vc[0] +vr[0]*0.875 -vf[0] +vt[0];
	tc[1] = vc[1] +vr[1]*0.875 -vf[1] +vt[1];
	tc[2] = vc[2] +vr[2]*0.875 -vf[2] +vt[2];
	carvesolid_cylinder(ctx, 0x800000, tc, tr, tt, tf);
	tc[0] = vc[0] +vr[0]-tr[0] -vf[0] +vt[0];
	tc[1] = vc[1] +vr[1]-tr[1] -vf[1] +vt[1];
	tc[2] = vc[2] +vr[2]-tr[2] -vf[2] +vt[2];
	carvesolid_prism4(ctx, 0x800000, tc, tr, tf, tt);

	//l,f
	tc[0] = vc[0] -vr[0]*0.875 +vf[0] +vt[0];
	tc[1] = vc[1] -vr[1]*0.875 +vf[1] +vt[1];
	tc[2] = vc[2] -vr[2]*0.875 +vf[2] +vt[2];
	carvesolid_cylinder(ctx, 0x800000, tc, tr, tt, tf);
	tc[0] = vc[0] -vr[0]+tr[0] +vf[0] +vt[0];
	tc[1] = vc[1] -vr[1]+tr[1] +vf[1] +vt[1];
	tc[2] = vc[2] -vr[2]+tr[2] +vf[2] +vt[2];
	carvesolid_prism4(ctx, 0x800000, tc, tr, tf, tt);

	//r,f
	tc[0] = vc[0] +vr[0]*0.875 +vf[0] +vt[0];
	tc[1] = vc[1] +vr[1]*0.875 +vf[1] +vt[1];
	tc[2] = vc[2] +vr[2]*0.875 +vf[2] +vt[2];
	carvesolid_cylinder(ctx, 0x800000, tc, tr, tt, tf);
	tc[0] = vc[0] +vr[0]-tr[0] +vf[0] +vt[0];
	tc[1] = vc[1] +vr[1]-tr[1] +vf[1] +vt[1];
	tc[2] = vc[2] +vr[2]-tr[2] +vf[2] +vt[2];
	carvesolid_prism4(ctx, 0x800000, tc, tr, tf, tt);


	//leg
	tr[0] = vr[0]/16;
	tr[1] = vr[1]/16;
	tr[2] = vr[2]/16;
	tf[0] = vf[0]/16;
	tf[1] = vf[1]/16;
	tf[2] = vf[2]/16;

	//l,b
	tt[0] = vt[0]/16;
	tt[1] = vt[1]/16;
	tt[2] = vt[2]/16;
	tc[0] = vc[0] -vr[0] -vf[0] +vt[0];
	tc[1] = vc[1] -vr[1] -vf[1] +vt[1];
	tc[2] = vc[2] -vr[2] -vf[2] +vt[2];
	carvesolid_cylinder(ctx, 0x008000, tc, tf, tt, tr);
	tc[0] = vc[0] -vr[0] -vf[0] +vt[0]/2;
	tc[1] = vc[1] -vr[1] -vf[1] +vt[1]/2;
	tc[2] = vc[2] -vr[2] -vf[2] +vt[2]/2;
	carvesolid_cylinder(ctx, 0x000080, tc, tf, tt, tr);

	tt[0] = vt[0]/4;
	tt[1] = vt[1]/4;
	tt[2] = vt[2]/4;
	tc[0] = vc[0] - vr[0] - vf[0] + vt[0]*0.75;
	tc[1] = vc[1] - vr[1] - vf[1] + vt[1]*0.75;
	tc[2] = vc[2] - vr[2] - vf[2] + vt[2]*0.75;
	carvesolid_cylinder(ctx, 0x008000, tc, tr, tf, tt);
	tc[0] = vc[0] - vr[0] - vf[0] + tt[0];
	tc[1] = vc[1] - vr[1] - vf[1] + tt[1];
	tc[2] = vc[2] - vr[2] - vf[2] + tt[2];
	carvesolid_cylinder(ctx, 0x000080, tc, tr, tf, tt);

	//r,b
	tt[0] = vt[0]/4;
	tt[1] = vt[1]/4;
	tt[2] = vt[2]/4;
	tc[0] = vc[0] + vr[0] - vf[0] + vt[0]*0.75;
	tc[1] = vc[1] + vr[1] - vf[1] + vt[1]*0.75;
	tc[2] = vc[2] + vr[2] - vf[2] + vt[2]*0.75;
	carvesolid_cylinder(ctx, 0x008000, tc, tr, tf, tt);
	tc[0] = vc[0] + vr[0] - vf[0] + tt[0];
	tc[1] = vc[1] + vr[1] - vf[1] + tt[1];
	tc[2] = vc[2] + vr[2] - vf[2] + tt[2];
	carvesolid_cylinder(ctx, 0x000080, tc, tr, tf, tt);

	//l,f
	tt[0] = vt[0]/4;
	tt[1] = vt[1]/4;
	tt[2] = vt[2]/4;
	tc[0] = vc[0] - vr[0] + vf[0] + vt[0]*0.75;
	tc[1] = vc[1] - vr[1] + vf[1] + vt[1]*0.75;
	tc[2] = vc[2] - vr[2] + vf[2] + vt[2]*0.75;
	carvesolid_cylinder(ctx, 0x008000, tc, tr, tf, tt);
	tc[0] = vc[0] - vr[0] + vf[0] + tt[0];
	tc[1] = vc[1] - vr[1] + vf[1] + tt[1];
	tc[2] = vc[2] - vr[2] + vf[2] + tt[2];
	carvesolid_cylinder(ctx, 0x000080, tc, tr, tf, tt);

	//r,f
	tt[0] = vt[0]/4;
	tt[1] = vt[1]/4;
	tt[2] = vt[2]/4;
	tc[0] = vc[0] + vr[0] + vf[0] + vt[0]*0.75;
	tc[1] = vc[1] + vr[1] + vf[1] + vt[1]*0.75;
	tc[2] = vc[2] + vr[2] + vf[2] + vt[2]*0.75;
	carvesolid_cylinder(ctx, 0x008000, tc, tr, tf, tt);
	tc[0] = vc[0] + vr[0] + vf[0] + tt[0];
	tc[1] = vc[1] + vr[1] + vf[1] + tt[1];
	tc[2] = vc[2] + vr[2] + vf[2] + tt[2];
	carvesolid_cylinder(ctx, 0x000080, tc, tr, tf, tt);
}
static void robodog_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void robodog_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void robodog_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void robodog_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void robodog_draw(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)robodog_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)robodog_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)robodog_draw_html(act, pin, win, sty);
	else if(fmt == _json_)robodog_draw_json(act, pin, win, sty);
	else robodog_draw_pixel(act, pin, win, sty);
}




//-4: wnd, area
//-3: cam, 0
//-2: cam, 0
//-1: world, geom of cam
static void robodog_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	//wnd -> cam
	struct entity* wnd;struct style* area;

	//cam -> world
	struct entity* wrd;struct style* camg;

	//scene -> robodog
	struct entity* scn;struct style* geom;
	struct entity* act;struct style* part;

	if(stack){
		act = self->pchip;part = self->pfoot;
		scn = peer->pchip;geom = peer->pfoot;
		wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		if('v' == len)robodog_draw_vbo(act,part, scn,geom, wnd,area);
	}
}
static void robodog_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void robodog_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void robodog_start(struct halfrel* self, struct halfrel* peer)
{
}




static void robodog_modify(struct entity* act)
{
}
static void robodog_search(struct entity* act)
{
}
static void robodog_delete(struct entity* act)
{
	if(0 == act)return;
	//if(_copy_ == act->type)memorydelete(act->buf);
}
static void robodog_create(struct entity* act)
{
	if(0 == act)return;
	//if(_orig_ == act->type)act->buf = buffer;
	//if(_copy_ == act->type)act->buf = memorycreate(256, 0);
}




void robodog_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('r', 'o', 'b', 'o', 'd', 'o', 'g', 0);

	p->oncreate = (void*)robodog_create;
	p->ondelete = (void*)robodog_delete;
	p->onsearch = (void*)robodog_search;
	p->onmodify = (void*)robodog_modify;

	p->onstart = (void*)robodog_start;
	p->onstop  = (void*)robodog_stop;
	p->onread  = (void*)robodog_read;
	p->onwrite = (void*)robodog_write;
}
