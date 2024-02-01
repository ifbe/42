#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151




static void dna_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void dna_draw_gl41(
	_obj* act, struct style* slot,
	_obj* wrl, struct style* geom,
	_obj* wnd, struct style* area)
{
	int z;
	float a,c,s;
	float A,C,S;
	vec3 tc,tr,tf,tu;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;
	gl41line_prism4(wnd, 0xffffff, vc, vr ,vf, vu);

	tf[0] = vf[0] / 16;
	tf[1] = vf[1] / 16;
	tf[2] = vf[2] / 16;
	tu[0] = vu[0] / 16;
	tu[1] = vu[1] / 16;
	tu[2] = vu[2] / 16;
	for(z=-6;z<=6;z++)
	{
		a = 2*PI*z/6;
		c = getcos(a)/2;
		s = getsin(a)/2;

		tc[0] = vc[0] + vr[0]*c + vf[0]*s + vu[0]*z/6.0;
		tc[1] = vc[1] + vr[1]*c + vf[1]*s + vu[1]*z/6.0;
		tc[2] = vc[2] + vr[2]*c + vf[2]*s + vu[2]*z/6.0;
		tr[0] = vr[0] / 16;
		tr[1] = vr[1] / 16;
		tr[2] = vr[2] / 16;
		gl41solid_sphere(wnd, 0xff0000, tc, tr, tf, tu);

		tc[0] = vc[0] - vr[0]*c - vf[0]*s + vu[0]*z/6.0;
		tc[1] = vc[1] - vr[1]*c - vf[1]*s + vu[1]*z/6.0;
		tc[2] = vc[2] - vr[2]*c - vf[2]*s + vu[2]*z/6.0;
		gl41solid_sphere(wnd, 0x0000ff, tc, tr, tf, tu);
	}

	for(z=-6;z<=5;z++)
	{
		a = 2*PI*z/6;
		c = getcos(a)/2;
		s = getsin(a)/2;
		A = 2*PI*(z+1)/6;
		C = getcos(A)/2;
		S = getsin(A)/2;

		tc[0] = vc[0] + vr[0]*c + vf[0]*s + vu[0]*z/6.0;
		tc[1] = vc[1] + vr[1]*c + vf[1]*s + vu[1]*z/6.0;
		tc[2] = vc[2] + vr[2]*c + vf[2]*s + vu[2]*z/6.0;
		tr[0] = vc[0] + vr[0]*C + vf[0]*S + vu[0]*(z+1)/6.0;
		tr[1] = vc[1] + vr[1]*C + vf[1]*S + vu[1]*(z+1)/6.0;
		tr[2] = vc[2] + vr[2]*C + vf[2]*S + vu[2]*(z+1)/6.0;
		gl41line(wnd, 0xff0000, tc, tr);

		tc[0] = vc[0] - vr[0]*c - vf[0]*s + vu[0]*z/6.0;
		tc[1] = vc[1] - vr[1]*c - vf[1]*s + vu[1]*z/6.0;
		tc[2] = vc[2] - vr[2]*c - vf[2]*s + vu[2]*z/6.0;
		tr[0] = vc[0] - vr[0]*C - vf[0]*S + vu[0]*(z+1)/6.0;
		tr[1] = vc[1] - vr[1]*C - vf[1]*S + vu[1]*(z+1)/6.0;
		tr[2] = vc[2] - vr[2]*C - vf[2]*S + vu[2]*(z+1)/6.0;
		gl41line(wnd, 0x0000ff, tc, tr);
	}
}
static void dna_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void dna_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void dna_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void dna_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void dna_read_byworld_bycam_bywnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;

	dna_draw_gl41(ent,slot, wor,geom, wnd,area);
}
static void dna_taking(_obj* ent,void* slot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
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
		break;
	default:
		dna_read_byworld_bycam_bywnd(ent,slot, stack,sp);
	}
}
static void dna_giving(_obj* ent,void* slot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void dna_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void dna_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void dna_search(_obj* act)
{
}
static void dna_modify(_obj* act)
{
}
static void dna_delete(_obj* act)
{
	if(0 == act)return;
	//if(_copy_ == act->type)memoryfree(act->buf);
}
static void dna_create(_obj* act)
{
	if(0 == act)return;
	//if(_orig_ == act->type)act->buf = buffer;
	//if(_copy_ == act->type)act->buf = memoryalloc(256, 0);
}




void dna_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex32('d', 'n', 'a', 0);

	p->oncreate = (void*)dna_create;
	p->ondelete = (void*)dna_delete;
	p->onreader = (void*)dna_search;
	p->onwriter = (void*)dna_modify;

	p->onattach = (void*)dna_attach;
	p->ondetach = (void*)dna_detach;
	p->ontaking = (void*)dna_taking;
	p->ongiving = (void*)dna_giving;
}
