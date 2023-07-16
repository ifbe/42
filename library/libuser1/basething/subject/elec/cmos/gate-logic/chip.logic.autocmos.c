#include "libuser.h"
void gl41data_before(_obj* wnd);
void gl41data_after(_obj* wnd);
void gl41data_01cam(_obj* wnd);
void gl41data_convert(_obj* wnd, struct style* area, struct event* ev, vec3 v);
void gl41line_pmos(_obj* wnd, u32 irgb, u32 orgb, vec3 vc, vec3 vr, vec3 vf, vec3 vt);
void gl41line_nmos(_obj* wnd, u32 irgb, u32 orgb, vec3 vc, vec3 vr, vec3 vf, vec3 vt);




static void autocmos_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	int cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->fs.vc[0];
		cy = sty->fs.vc[1];
		ww = sty->fs.vr[0];
		hh = sty->fs.vf[1];
	}
	else
	{
		cx = win->whdf.width/2;
		cy = win->whdf.height/2;
		ww = win->whdf.width/2;
		hh = win->whdf.height/2;
	}
}
static void autocmos_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void autocmos_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void autocmos_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void autocmos_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void autocmos_draw_gl41(
	_obj* act, struct style* slot,
	_obj* win, struct style* geom,
	_obj* ctx, struct style* area)
{
	int w,h;
	int x,y,j;
	vec3 tc,tr,tf,tt;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
	gl41opaque_rect(ctx, 0x40404040, vc, vr, vf);

	//vcc
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j] +vf[j];
		tr[j] = vc[j] +vr[j] +vf[j];
	}
	gl41line(ctx, 0xff0000, tc, tr);

	//gnd
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j] -vf[j];
		tr[j] = vc[j] +vr[j] -vf[j];
	}
	gl41line(ctx, 0x0000ff, tc, tr);

	//nmos
	w = act->whdf.ix0;
	h = act->whdf.iy0;
	for(j=0;j<3;j++){
		tr[j] = vr[j]/w;
		tf[j] = vf[j]/h/2;
	}
	for(y=0;y<h;y++){
		for(x=0;x<w;x++){
			for(j=0;j<3;j++){
				tc[j] = vc[j] +vr[j]*(x*2-w+1)/w -vf[j]+vf[j]*(2*y+1)/h/2;
			}
			gl41line_nmos(ctx, 0xffffff, 0xffffff, tc,tr,tf,vt);
		}
	}

	//pmos
	w = act->whdf.ixn;
	h = act->whdf.iyn;
	for(j=0;j<3;j++){
		tr[j] = vr[j]/w;
		tf[j] = vf[j]/h/2;
	}
	for(y=0;y<h;y++){
		for(x=0;x<w;x++){
			for(j=0;j<3;j++){
				tc[j] = vc[j] +vr[j]*(x*2-w+1)/w +vf[j]-vf[j]*(2*y+1)/h/2;
			}
			gl41line_pmos(ctx, 0xffffff, 0xffffff, tc,tr,tf,vt);
		}
	}
}




static void autocmos_wrl_cam_wnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	autocmos_draw_gl41(ent,slot, wor,geom, wnd,area);
}
static void autocmos_wrl_wnd(_obj* ent,void* foot, _syn* stack,int sp)
{
}
static void autocmos_wnd(_obj* ent,void* foot, _syn* stack,int sp)
{
	_obj* wnd;struct style* area;
	wnd = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	struct fstyle fs;
	fs.vc[0] = 0.0;fs.vc[1] = 0.0;fs.vc[2] = 0.0;
	fs.vr[0] = 1.0;fs.vr[1] = 0.0;fs.vr[2] = 0.0;
	fs.vf[0] = 0.0;fs.vf[1] = 1.0;fs.vf[2] = 0.0;
	fs.vt[0] = 0.0;fs.vt[1] = 0.0;fs.vt[2] = 1.0;

	gl41data_before(wnd);
	autocmos_draw_gl41(ent, 0, 0,(void*)&fs, wnd,area);
	gl41data_01cam(wnd);
	gl41data_after(wnd);
}




static void autocmos_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
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
		autocmos_wnd(ent,foot, stack,sp);break;
	default:
		autocmos_wrl_cam_wnd(ent,foot, stack,sp);break;
		break;
	}
}
static void autocmos_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	//say("@autocmos_write:%x\n",buf[0]);
}
static void autocmos_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void autocmos_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void autocmos_search(_obj* act, u8* buf)
{
}
static void autocmos_modify(_obj* act, u8* buf)
{
}
static void autocmos_delete(_obj* act, u8* buf)
{
}
static void autocmos_create(_obj* act, u8* buf)
{
	act->whdf.ix0 = 3;
	act->whdf.iy0 = 4;
	act->whdf.ixn = 4;
	act->whdf.iyn = 3;
}




void autocmos_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('a','u','t','o','c','m','o','s');

	p->oncreate = (void*)autocmos_create;
	p->ondelete = (void*)autocmos_delete;
	p->onreader = (void*)autocmos_search;
	p->onwriter = (void*)autocmos_modify;

	p->onattach = (void*)autocmos_attach;
	p->ondetach = (void*)autocmos_detach;
	p->ontaking = (void*)autocmos_taking;
	p->ongiving = (void*)autocmos_giving;
}
