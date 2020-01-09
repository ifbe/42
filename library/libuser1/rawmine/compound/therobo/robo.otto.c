#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151




static void otto_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void otto_draw_gl41(
	struct entity* act, struct style* part,
	struct entity* scn, struct style* geom,
	struct entity* ctx, struct style* area)
{
	vec3 tc,tr,tf,tt;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vt = geom->f.vt;
	carveline_prism4(ctx, 0xff00ff, vc,vr,vf,vt);

	tc[0] = vc[0] + vt[0]*0.75;
	tc[1] = vc[1] + vt[1]*0.75;
	tc[2] = vc[2] + vt[2]*0.75;
	tr[0] = vr[0]/2;
	tr[1] = vr[1]/2;
	tr[2] = vr[2]/2;
	tf[0] = vf[0]/2;
	tf[1] = vf[1]/2;
	tf[2] = vf[2]/2;
	tt[0] = vt[0]/4;
	tt[1] = vt[1]/4;
	tt[2] = vt[2]/4;
	carvesolid_prism4(ctx, 0x808080, tc,tr,tf,tt);

	//arm
	tr[0] = vr[0]/8;
	tr[1] = vr[1]/8;
	tr[2] = vr[2]/8;
	tf[0] = vf[0]/8;
	tf[1] = vf[1]/8;
	tf[2] = vf[2]/8;
	tt[0] = vt[0]/4;
	tt[1] = vt[1]/4;
	tt[2] = vt[2]/4;

	//l arm
	tc[0] = vc[0] -vr[0]+tr[0] + vt[0]*0.5+tt[0];
	tc[1] = vc[1] -vr[1]+tr[1] + vt[1]*0.5+tt[1];
	tc[2] = vc[2] -vr[2]+tr[2] + vt[2]*0.5+tt[2];
	carvesolid_prism4(ctx, 0x808080, tc,tr,tf,tt);

	//r arm
	tc[0] = vc[0] +vr[0]-tr[0] + vt[0]*0.5+tt[0];
	tc[1] = vc[1] +vr[1]-tr[1] + vt[1]*0.5+tt[1];
	tc[2] = vc[2] +vr[2]-tr[2] + vt[2]*0.5+tt[2];
	carvesolid_prism4(ctx, 0x808080, tc,tr,tf,tt);


	//arm
	tr[0] = vr[0]/8;
	tr[1] = vr[1]/8;
	tr[2] = vr[2]/8;
	tf[0] = vf[0]/8;
	tf[1] = vf[1]/8;
	tf[2] = vf[2]/8;
	tt[0] = vt[0]/4;
	tt[1] = vt[1]/4;
	tt[2] = vt[2]/4;

	//l leg
	tc[0] = vc[0] - vr[0]*0.25 + vt[0]*0.25;
	tc[1] = vc[1] - vr[1]*0.25 + vt[1]*0.25;
	tc[2] = vc[2] - vr[2]*0.25 + vt[2]*0.25;
	carvesolid_prism4(ctx, 0x808080, tc,tr,tf,tt);

	//r leg
	tc[0] = vc[0] + vr[0]*0.25 + vt[0]*0.25;
	tc[1] = vc[1] + vr[1]*0.25 + vt[1]*0.25;
	tc[2] = vc[2] + vr[2]*0.25 + vt[2]*0.25;
	carvesolid_prism4(ctx, 0x808080, tc,tr,tf,tt);


	//foot
	tr[0] = vr[0]*0.375;
	tr[1] = vr[1]*0.375;
	tr[2] = vr[2]*0.375;
	tf[0] = vf[0]/4;
	tf[1] = vf[1]/4;
	tf[2] = vf[2]/4;
	tt[0] = vt[0]/64;
	tt[1] = vt[1]/64;
	tt[2] = vt[2]/64;

	//l foot
	tc[0] = vc[0] - vr[0]+tr[0] + tt[0];
	tc[1] = vc[1] - vr[1]+tr[1] + tt[1];
	tc[2] = vc[2] - vr[2]+tr[2] + tt[2];
	carvesolid_prism4(ctx, 0x808080, tc, tr,tf,tt);

	//l foot
	tc[0] = vc[0] + vr[0]-tr[0] + tt[0];
	tc[1] = vc[1] + vr[1]-tr[1] + tt[1];
	tc[2] = vc[2] + vr[2]-tr[2] + tt[2];
	carvesolid_prism4(ctx, 0x808080, tc, tr,tf,tt);
}
static void otto_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void otto_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void otto_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void otto_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void otto_draw(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)otto_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)otto_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)otto_draw_html(act, pin, win, sty);
	else if(fmt == _json_)otto_draw_json(act, pin, win, sty);
	else otto_draw_pixel(act, pin, win, sty);
}




//-4: wnd, area
//-3: cam, 0
//-2: cam, 0
//-1: world, geom of cam
static void otto_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	//wnd -> cam
	struct entity* wnd;struct style* area;

	//cam -> world
	struct entity* wrd;struct style* camg;

	//scene -> otto
	struct entity* scn;struct style* geom;
	struct entity* act;struct style* part;

	if(stack){
		act = self->pchip;part = self->pfoot;
		scn = peer->pchip;geom = peer->pfoot;
		wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		if('v' == len)otto_draw_gl41(act,part, scn,geom, wnd,area);
	}
}
static void otto_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void otto_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void otto_start(struct halfrel* self, struct halfrel* peer)
{
}




static void otto_modify(struct entity* act)
{
}
static void otto_search(struct entity* act)
{
}
static void otto_delete(struct entity* act)
{
	if(0 == act)return;
	//if(_copy_ == act->type)memorydelete(act->buf);
}
static void otto_create(struct entity* act)
{
	if(0 == act)return;
	//if(_orig_ == act->type)act->buf = buffer;
	//if(_copy_ == act->type)act->buf = memorycreate(256, 0);
}




void otto_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex32('o', 't', 't', 'o');

	p->oncreate = (void*)otto_create;
	p->ondelete = (void*)otto_delete;
	p->onsearch = (void*)otto_search;
	p->onmodify = (void*)otto_modify;

	p->onstart = (void*)otto_start;
	p->onstop  = (void*)otto_stop;
	p->onread  = (void*)otto_read;
	p->onwrite = (void*)otto_write;
}
