#include "libuser.h"




static void status_draw_gl41(
	_obj* act, struct style* slot,
	_obj* win, struct style* geom,
	_obj* ctx, struct style* area)
{
	float* vc = geom->fshape.vc;
	float* vr = geom->fshape.vr;
	float* vf = geom->fshape.vf;
	float* vt = geom->fshape.vt;
	gl41solid_rect(ctx,0x808080, vc,vr,vf);
}
static void status_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void status_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void status_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void status_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void status_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void status_read_bycam(_obj* ent,void* slot, _syn* stack,int sp, p64 arg,int key)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	status_draw_gl41(ent,slot, wor,geom, wnd,area);
}
static void status_read_byuiux(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* uuu;struct style* area;
	_obj* wnd;struct style* rect;
	uuu = stack[sp-2].pchip;area = stack[sp-2].pfoot;
	wnd = stack[sp-4].pchip;rect = stack[sp-4].pfoot;

	float x0,y0,xn,yn;
	x0 = area->fs.vc[0];
	y0 = area->fs.vc[1];
	xn = area->fs.vc[0]+area->fs.vq[0];
	yn = area->fs.vc[1]+area->fs.vq[1];
	//logtoall("%f,%f,%f,%f\n",x0,y0,dx,dy);

	struct fstyle fs;
	fs.vc[0] = (x0+xn)/2;fs.vc[1] = (y0+yn)/2;fs.vc[2] = 0.5;
	fs.vr[0] = (xn-x0)/2;fs.vr[1] =       0.0;fs.vr[2] = 0.0;
	fs.vf[0] =       0.0;fs.vf[1] = (yn-y0)/2;fs.vf[2] = 0.0;
	fs.vt[0] =       0.0;fs.vt[1] =       0.0;fs.vt[2] =-0.5;

	status_draw_gl41(ent,slot, 0,(void*)&fs, wnd,rect);
}
static void status_read_bywnd(_obj* ent,struct style* slot, _obj* wnd,struct style* area)
{
}




static void status_taking(_obj* ent,void* slot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	_obj* wnd = stack[sp-2].pchip;
	struct style* area = stack[sp-2].pfoot;
//logtoall("fmt=%.8s\n", &sup->type);

	switch(wnd->type){
	case _wnd_:
		status_read_bywnd(ent,slot, wnd,area);
		break;
	case _virtual_:
		status_read_byuiux(ent,slot, stack,sp);
		break;
	default:
		status_read_bycam(ent,slot, stack,sp, arg,key);
		break;
	}
}
static void status_giving(_obj* ent,void* slot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void status_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void status_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void status_search(_obj* act)
{
}
static void status_modify(_obj* act)
{
}
static void status_delete(_obj* act)
{
}
static void status_create(_obj* act, u8* arg)
{
}




void status_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex64('s','t','a','t','u','s', 0, 0);

	p->oncreate = (void*)status_create;
	p->ondelete = (void*)status_delete;
	p->onreader = (void*)status_search;
	p->onwriter = (void*)status_modify;

	p->onattach = (void*)status_attach;
	p->ondetach = (void*)status_detach;
	p->ontaking = (void*)status_taking;
	p->ongiving = (void*)status_giving;
}
