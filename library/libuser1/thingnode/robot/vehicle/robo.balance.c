#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151




static void balance_draw_pixel(
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
static void balance_draw_gl41(
	_obj* act, struct style* part,
	_obj* scn, struct style* geom,
	_obj* ctx, struct style* area)
{
	int j;
	vec3 tc,tr,tf,tt;
	float* vc = geom->fshape.vc;
	float* vr = geom->fshape.vr;
	float* vf = geom->fshape.vf;
	float* vt = geom->fshape.vt;
	gl41line_prism4(ctx, 0xffffff, vc, vr, vf, vt);

	//board
	float angle = getsin(act->whdf.fy0)*PI/6;
	act->whdf.fy0 += 0.01;

	for(j=0;j<3;j++){
		tr[j] = vr[j]*4/8;
		tf[j] = vf[j]/4;
		tt[j] = vt[j]*3/4;
	}
	quaternion_operation(tf, vr, angle);
	quaternion_operation(tt, vr, angle);
	for(j=0;j<3;j++){
		tc[j] = vc[j]-vt[j]/2+tt[j];
	}
	gl41solid_prism4(ctx, 0x808080, tc,tr,tf,tt);

	//shaft
	for(j=0;j<3;j++){
		tr[j] = vr[j];
		tf[j] = vf[j]/16;
		tt[j] = vt[j]/16;
		tc[j] = vc[j]-vt[j]/2;
	}
	gl41solid_cylinder(ctx, 0x404040, tc, tf, tt, tr);

	//left wheel
	for(j=0;j<3;j++){
		tf[j] = vf[j]/2;
		tt[j] = vt[j]/2;
		tr[j] = vr[j]/8;
		tc[j] = vc[j] -vr[j]+tr[j] -vt[j]/2;
	}
	gl41solid_cylinder(ctx, 0x404040, tc, tf, tt, tr);

	//right wheel
	for(j=0;j<3;j++){
		tf[j] = vf[j]/2;
		tt[j] = vt[j]/2;
		tr[j] = vr[j]/8;
		tc[j] = vc[j] +vr[j]-tr[j] -vt[j]/2;
	}
	gl41solid_cylinder(ctx, 0x404040, tc, tf, tt, tr);
}
static void balance_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void balance_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void balance_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void balance_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




void balance_write_euler(_obj* act, float* f)
{
	act->whdf.fx0 = f[0];
	act->whdf.fy0 = f[1];
	act->whdf.fz0 = f[2];
}
void balance_write_quaternion(_obj* act, float* q)
{
}




static void balance_wrl_cam_wnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	balance_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void balance_taking(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
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
		break;
	default:
		balance_wrl_cam_wnd(ent,foot, stack,sp);
		break;
	}
}
static void balance_giving(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	balance_write_euler(ent, buf);
}
static void balance_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void balance_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void balance_modify(_obj* act)
{
}
static void balance_search(_obj* act)
{
}
static void balance_delete(_obj* act)
{
	if(0 == act)return;
	//if(_copy_ == act->type)memorydelete(act->buf);
}
static void balance_create(_obj* act)
{
	if(0 == act)return;
	act->whdf.fy0 = PI/4;
}




void balance_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('b','a','l','a','n','c','e', 0);

	p->oncreate = (void*)balance_create;
	p->ondelete = (void*)balance_delete;
	p->onreader = (void*)balance_search;
	p->onwriter = (void*)balance_modify;

	p->onattach = (void*)balance_attach;
	p->ondetach = (void*)balance_detach;
	p->ontaking = (void*)balance_taking;
	p->ongiving = (void*)balance_giving;
}
