#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151




static void otto_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void otto_draw_gl41(
	_obj* act, struct style* part,
	_obj* scn, struct style* geom,
	_obj* ctx, struct style* area)
{
	vec3 tc,tr,tf,tt;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
	gl41line_prism4(ctx, 0xff00ff, vc,vr,vf,vt);

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
	gl41solid_prism4(ctx, 0x808080, tc,tr,tf,tt);

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
	gl41solid_prism4(ctx, 0x808080, tc,tr,tf,tt);

	//r arm
	tc[0] = vc[0] +vr[0]-tr[0] + vt[0]*0.5+tt[0];
	tc[1] = vc[1] +vr[1]-tr[1] + vt[1]*0.5+tt[1];
	tc[2] = vc[2] +vr[2]-tr[2] + vt[2]*0.5+tt[2];
	gl41solid_prism4(ctx, 0x808080, tc,tr,tf,tt);


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
	gl41solid_prism4(ctx, 0x808080, tc,tr,tf,tt);

	//r leg
	tc[0] = vc[0] + vr[0]*0.25 + vt[0]*0.25;
	tc[1] = vc[1] + vr[1]*0.25 + vt[1]*0.25;
	tc[2] = vc[2] + vr[2]*0.25 + vt[2]*0.25;
	gl41solid_prism4(ctx, 0x808080, tc,tr,tf,tt);


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
	gl41solid_prism4(ctx, 0x808080, tc, tr,tf,tt);

	//l foot
	tc[0] = vc[0] + vr[0]-tr[0] + tt[0];
	tc[1] = vc[1] + vr[1]-tr[1] + tt[1];
	tc[2] = vc[2] + vr[2]-tr[2] + tt[2];
	gl41solid_prism4(ctx, 0x808080, tc, tr,tf,tt);
}
static void otto_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void otto_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void otto_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void otto_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void otto_read_byworld_bycam_bywnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	otto_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void otto_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	//foot defined behavior
	switch(stack[sp-1].foottype){
	}

	//caller defined behavior
	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(caller->type){
	case _wnd_:
		break;
	default:
		otto_read_byworld_bycam_bywnd(ent,foot, stack,sp);
		break;
	}
}
static void otto_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void otto_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void otto_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void otto_modify(_obj* act)
{
}
static void otto_search(_obj* act)
{
}
static void otto_delete(_obj* act)
{
	if(0 == act)return;
	//if(_copy_ == act->type)memoryfree(act->buf);
}
static void otto_create(_obj* act)
{
	if(0 == act)return;
	//if(_orig_ == act->type)act->buf = buffer;
	//if(_copy_ == act->type)act->buf = memoryalloc(256, 0);
}




void otto_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex32('o', 't', 't', 'o');

	p->oncreate = (void*)otto_create;
	p->ondelete = (void*)otto_delete;
	p->onreader = (void*)otto_search;
	p->onwriter = (void*)otto_modify;

	p->onattach = (void*)otto_attach;
	p->ondetach = (void*)otto_detach;
	p->ontaking = (void*)otto_taking;
	p->ongiving = (void*)otto_giving;
}
