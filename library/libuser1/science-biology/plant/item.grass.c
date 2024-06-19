#include "libuser.h"




static void grass_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void grass_draw_gl41(
	_obj* act, struct style* part,
	_obj* win, struct style* geom,
	_obj* ctx, struct style* area)
{
	int j,k;
	vec3 v0,v1,v2;
	float x0,y0,z0,rx,ry,rz,fx,fy,fz;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;

	float* f = act->listptr.buf0;
	for(k=0;k<8192;k++){
		x0 = f[6*k+0];
		y0 = f[6*k+1];
		z0 = f[6*k+2];
		rx = f[6*k+3];
		ry = f[6*k+4];
		rz = f[6*k+5];
		fx =-ry/4;
		fy = rx/4;
		fz = rz;
		for(j=0;j<3;j++){
			v0[j] = vc[j] + vr[j]*x0 + vf[j]*y0;
			v1[j] = vc[j] + vr[j]*(x0+rx) + vf[j]*(y0+ry) + vt[j]*rz;
			v2[j] = vc[j] + vr[j]*(x0+rx/20+fx) + vf[j]*(y0+ry/20+fy);
		}
		gl41solid_triangle(ctx,0x008000, v0,v1,v2);

		for(j=0;j<3;j++){
			v0[j] = vc[j] + vr[j]*x0 + vf[j]*y0;
			v1[j] = vc[j] + vr[j]*(x0+rx) + vf[j]*(y0+ry) + vt[j]*rz;
			v2[j] = vc[j] + vr[j]*(x0+rx/20-fx) + vf[j]*(y0+ry/20-fy);
		}
		gl41solid_triangle(ctx,0x008000, v1,v0,v2);
	}
}
static void grass_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void grass_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void grass_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void grass_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void grass_read_byworld_bycam_bywnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	grass_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void grass_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
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
	case _render_:
		break;
	default:
		grass_read_byworld_bycam_bywnd(ent,foot, stack,sp);
		break;
	}
}
static void grass_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void grass_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void grass_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void grass_search(_obj* act)
{
}
static void grass_modify(_obj* act)
{
}
static void grass_delete(_obj* act)
{
}
static void grass_create(_obj* act)
{
	int j,k;
	float* f = act->listptr.buf0 = memoryalloc(4*6*0x10000, 0);
	for(j=0;j<2048;j++){
		f[j*24+0] = (random_read()&0xffff)/32768.0 - 1.0;
		f[j*24+1] = (random_read()&0xffff)/32768.0 - 1.0;
		for(k=1;k<4;k++){
			f[(j*4+k)*6+0] = f[j*24+0];
			f[(j*4+k)*6+1] = f[j*24+1];
		}

		for(k=0;k<4;k++){
			f[(j*4+k)*6+3] = (random_read()&0x7ff)/65536.0 - 0x400/65536.0;
			f[(j*4+k)*6+4] = (random_read()&0x7ff)/65536.0 - 0x400/65536.0;
			f[(j*4+k)*6+5] = (random_read()&0xffff)/65536.0;
		}
	}
}




void grass_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex64('g','r','a','s','s', 0, 0, 0);

	p->oncreate = (void*)grass_create;
	p->ondelete = (void*)grass_delete;
	p->onreader = (void*)grass_search;
	p->onwriter = (void*)grass_modify;

	p->onattach = (void*)grass_attach;
	p->ondetach = (void*)grass_detach;
	p->ontaking = (void*)grass_taking;
	p->ongiving = (void*)grass_giving;
}
