#include "libuser.h"




static void grass_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void grass_draw_gl41(
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	int j,k;
	vec3 v0,v1,v2;
	float x0,y0,z0,rx,ry,rz,fx,fy,fz;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vt = geom->f.vt;

	float* f = act->buf0;
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
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void grass_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void grass_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void grass_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void grass_read(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct style* slot;
	struct entity* scn;struct style* geom;
	struct entity* wnd;struct style* area;

	if(stack && ('v'==key)){
		slot = stack[sp-1].pfoot;
		scn = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		grass_draw_gl41(ent,slot, scn,geom, wnd,area);
	}
}
static void grass_write(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void grass_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void grass_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void grass_search(struct entity* act)
{
}
static void grass_modify(struct entity* act)
{
}
static void grass_delete(struct entity* act)
{
}
static void grass_create(struct entity* act)
{
	int j,k;
	float* f = act->buf0 = memorycreate(4*6*0x10000, 0);
	for(j=0;j<2048;j++){
		f[j*24+0] = (getrandom()&0xffff)/32768.0 - 1.0;
		f[j*24+1] = (getrandom()&0xffff)/32768.0 - 1.0;
		for(k=1;k<4;k++){
			f[(j*4+k)*6+0] = f[j*24+0];
			f[(j*4+k)*6+1] = f[j*24+1];
		}

		for(k=0;k<4;k++){
			f[(j*4+k)*6+3] = (getrandom()&0x7ff)/65536.0 - 0x400/65536.0;
			f[(j*4+k)*6+4] = (getrandom()&0x7ff)/65536.0 - 0x400/65536.0;
			f[(j*4+k)*6+5] = (getrandom()&0xffff)/65536.0;
		}
	}
}




void grass_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('g','r','a','s','s', 0, 0, 0);

	p->oncreate = (void*)grass_create;
	p->ondelete = (void*)grass_delete;
	p->onsearch = (void*)grass_search;
	p->onmodify = (void*)grass_modify;

	p->onlinkup = (void*)grass_linkup;
	p->ondiscon = (void*)grass_discon;
	p->onread  = (void*)grass_read;
	p->onwrite = (void*)grass_write;
}
