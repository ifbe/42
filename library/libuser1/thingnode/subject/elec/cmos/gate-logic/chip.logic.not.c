#include "libuser.h"
void gl41line_pmos(_obj* wnd, u32 irgb, u32 orgb, vec3 vc, vec3 vr, vec3 vf, vec3 vt);
void gl41line_nmos(_obj* wnd, u32 irgb, u32 orgb, vec3 vc, vec3 vr, vec3 vf, vec3 vt);




static void not_draw_pixel(
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
static void not_draw_gl41(
	_obj* act, struct style* slot,
	_obj* win, struct style* geom,
	_obj* ctx, struct style* area)
{
	int j;
	vec3 tc,tr,tf,tt;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
	//gl41line_rect(ctx, 0x404040, vc, vr, vf);

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

	//+
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vf[j];
		tr[j] = tc[j] -tf[j]/2;
	}
	gl41line(ctx, 0xff0000, tc,tr);

	//-
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vf[j];
		tr[j] = tc[j] +tf[j]/2;
	}
	gl41line(ctx, 0x0000ff, tc,tr);

	u32 pcolor = act->whdf.ix0 ? 0xffffff : 0xff0000;
	u32 ncolor = act->whdf.ix0 ? 0x0000ff : 0xffffff;
	u32 icolor = act->whdf.ix0 ? 0xff0000 : 0x0000ff;
	u32 ocolor = act->whdf.iy0 ? 0xff0000 : 0x0000ff;

	//p
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vf[j]/2;
		tr[j] = vr[j]/2;
		tf[j] = vf[j]/2;
	}
	gl41line_pmos(ctx, icolor, pcolor, tc,tr,tf,vt);

	//n
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vf[j]/2;
		tr[j] = vr[j]/2;
		tf[j] = vf[j]/2;
	}
	gl41line_nmos(ctx, icolor, ncolor, tc,tr,tf,vt);

	//i
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]/2 -vf[j]/2;
		tr[j] = tc[j] +vf[j];
	}
	gl41line(ctx, icolor, tc,tr);

	//o
	for(j=0;j<3;j++)tc[j] = vc[j]+vr[j];
	gl41line(ctx, ocolor, vc,tc);
}
static void not_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void not_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void not_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void not_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void not_wrl_cam_wnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	not_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void not_taking(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
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
		not_wrl_cam_wnd(ent,foot, stack,sp);
		break;
	}
}
static void not_giving(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, u8* buf,int len)
{
	u8 tmp;
	say("@notgate_write:%x\n",buf[0]);

	if('a' == buf[0])ent->whdf.ix0 ^= 1;
	else if('0' == buf[0])ent->whdf.ix0 = 0;
	else if('1' == buf[0])ent->whdf.ix0 = 1;
	else return;

	ent->whdf.iy0 = !ent->whdf.ix0;
	tmp = ent->whdf.iy0 + 0x30;
	give_data_into_peer(ent,'o', stack,sp, 0,0, &tmp,1);
}
static void not_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void not_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void not_search(_obj* act, u8* buf)
{
}
static void not_modify(_obj* act, u8* buf)
{
}
static void not_delete(_obj* act, u8* buf)
{
}
static void not_create(_obj* act, u8* buf)
{
	act->whdf.ix0 = getrandom()&1;
	act->whdf.iy0 = !act->whdf.ix0;
}




void not_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex32('n','o','t', 0);

	p->oncreate = (void*)not_create;
	p->ondelete = (void*)not_delete;
	p->onreader = (void*)not_search;
	p->onwriter = (void*)not_modify;

	p->onattach = (void*)not_attach;
	p->ondetach = (void*)not_detach;
	p->ontaking = (void*)not_taking;
	p->ongiving = (void*)not_giving;
}
