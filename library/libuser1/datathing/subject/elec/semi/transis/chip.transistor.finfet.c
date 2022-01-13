#include "libuser.h"




static void finfet_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void finfet_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void finfet_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void finfet_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void finfet_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void finfet_draw_gl41(
	_obj* act, struct style* slot,
	_obj* scn, struct style* geom,
	_obj* wnd, struct style* area)
{
	int y,j;
	vec3 tc,tr,tf,tu;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;

	//body.base
	for(j=0;j<3;j++){
		tc[j] = vc[j] + vu[j]/8;
		tu[j] = vu[j] / 8;
	}
	gl41solid_prism4(wnd, 0x404040, tc, vr, vf, tu);

	//d
	for(j=0;j<3;j++){
		tr[j] = vr[j]/3;
		tf[j] = vf[j] / 16;
		tu[j] = vu[j] / 4;
	}
	for(y=-1;y<2;y++)
	{
		for(j=0;j<3;j++)tc[j] = vc[j] -vr[j]+tr[j] +y*vf[j]/2 +vu[j]/2;
		gl41solid_prism4(wnd, 0x404040, tc, tr, tf, tu);
	}

	//s
	for(y=-1;y<2;y++)
	{
		for(j=0;j<3;j++)tc[j] = vc[j] +vr[j]-tr[j] +y*vf[j]/2 +vu[j]/2;
		gl41solid_prism4(wnd, 0x404040, tc, tr, tf, tu);
	}

	//magic
	for(y=-1;y<2;y++)
	{
		for(j=0;j<3;j++)tc[j] = vc[j] +y*vf[j]/2 +vu[j]/2;
		gl41solid_prism4(wnd, 0x808080, tc, tr, tf, tu);
	}

	//oxide
	for(j=0;j<3;j++){
		tr[j] = vr[j] * 3 / 4;
		tf[j] = vf[j] * 3 / 32;
		tu[j] = vu[j] / 64;
	}
	for(y=-1;y<2;y++){
		for(j=0;j<3;j++)tc[j] = vc[j] +y*vf[j]/2 +vu[j]*3/4+tu[j];
		gl41opaque_prism4(wnd, 0x800000ff, tc,tr,tf,tu);
	}
	for(j=0;j<3;j++){
		tr[j] = vr[j] * 3 / 4;
		tf[j] = vf[j] / 64;
		tu[j] = vu[j] / 4;
	}
	for(y=-1;y<2;y++){
		for(j=0;j<3;j++)tc[j] = vc[j] +y*vf[j]/2-vf[j]/16-tf[j] +vu[j]*1/2;
		gl41opaque_prism4(wnd, 0x800000ff, tc,tr,tf,tu);
		for(j=0;j<3;j++)tc[j] = vc[j] +y*vf[j]/2+vf[j]/16+tf[j] +vu[j]*1/2;
		gl41opaque_prism4(wnd, 0x800000ff, tc,tr,tf,tu);
	}

	//gate.upper
	for(j=0;j<3;j++){
		tc[j] = vc[j] + vu[j]*7/8;
		tr[j] = vr[j]/2;
		tu[j] = vu[j]/8;
	}
	gl41opaque_prism4(wnd, 0x80ff0000, tc, tr, vf, tu);

	//gate.bottom
	for(j=0;j<3;j++){
		tf[j] = vf[j] * 3 / 16;
		tu[j] = vu[j] / 4;
	}
	for(y=-3;y<4;y+=2)
	{
		for(j=0;j<3;j++)tc[j] = vc[j] + y*vf[j]/4 + vu[j]/2;
		gl41opaque_prism4(wnd, 0x80ff0000, tc, tr, tf, tu);
	}
}




static void finfet_wrl_cam_wnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;

	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	finfet_draw_gl41(ent, slot, wor,geom, wnd,area);
}
static void finfet_wrl_wnd(_obj* ent,void* slot, _syn* stack,int sp)
{
}
static void finfet_wnd(_obj* ent,void* slot, _syn* stack,int sp)
{
}




static void finfet_taking(_obj* ent,void* slot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	//foot defined behavior
	switch(stack[sp-1].foottype){
	}

	//caller defined behavior
	switch(caller->hfmt){
	case _rgba_:
		break;
	case _gl41list_:
		finfet_wnd(ent,slot, stack,sp);break;
		break;
	default:
		finfet_wrl_cam_wnd(ent,slot, stack,sp);
	}
}
static void finfet_giving(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void finfet_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void finfet_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void finfet_search(_obj* act, u8* buf)
{
}
static void finfet_modify(_obj* act, u8* buf)
{
}
static void finfet_delete(_obj* act, u8* buf)
{
}
static void finfet_create(_obj* act, u8* buf)
{
}




void finfet_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('f','i','n','f','e','t', 0, 0);

	p->oncreate = (void*)finfet_create;
	p->ondelete = (void*)finfet_delete;
	p->onsearch = (void*)finfet_search;
	p->onmodify = (void*)finfet_modify;

	p->onattach = (void*)finfet_attach;
	p->ondetach = (void*)finfet_detach;
	p->ontaking = (void*)finfet_taking;
	p->ongiving = (void*)finfet_giving;
}
