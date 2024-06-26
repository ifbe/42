#include "libuser.h"
void gl41line_pmos(_obj* wnd, u32 irgb, u32 orgb, vec3 vc, vec3 vr, vec3 vf, vec3 vt);
void gl41line_nmos(_obj* wnd, u32 irgb, u32 orgb, vec3 vc, vec3 vr, vec3 vf, vec3 vt);




static void xnor_draw_gl41_12t(
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
		tc[j] = vc[j] -vr[j]/4 +vf[j];
		tr[j] = tc[j] -tf[j]/2;
	}
	gl41line(ctx, 0xff0000, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]/4 +vf[j];
		tr[j] = tc[j] -tf[j]/2;
	}
	gl41line(ctx, 0xff0000, tc,tr);

	//-
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]/4 -vf[j];
		tr[j] = tc[j] +tf[j]/2;
	}
	gl41line(ctx, 0x0000ff, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]/4 -vf[j];
		tr[j] = tc[j] +tf[j]/2;
	}
	gl41line(ctx, 0x0000ff, tc,tr);

	u32 acolor = act->whdf.ix0 ? 0xff0000 : 0x0000ff;
	u32 xcolor = act->whdf.ix0 ? 0x0000ff : 0xff0000;	//x=a'
	u32 bcolor = act->whdf.iy0 ? 0xff0000 : 0x0000ff;
	u32 ycolor = act->whdf.iy0 ? 0x0000ff : 0xff0000;	//y=b'
	u32 ocolor = act->whdf.iz0 ? 0xff0000 : 0x0000ff;
	u32 mcolor[8] = {
		0xffffff, 0xffffff,
		0xffffff, 0xffffff,
		0xffffff, 0xffffff,
		0xffffff, 0xffffff
	};
	if(0 == act->whdf.ix0){
		mcolor[0] = 0xff0000;
		if(0 == act->whdf.iy0)mcolor[2] = 0xff0000;
	}
	if(1 == act->whdf.ix0){
		mcolor[1] = 0xff0000;
		if(1 == act->whdf.iy0)mcolor[3] = 0xff0000;
	}
	if(0 == act->whdf.iy0){
		mcolor[6] = 0x0000ff;
		if(1 == act->whdf.ix0)mcolor[4] = 0x0000ff;
	}
	if(1 == act->whdf.iy0){
		mcolor[7] = 0x0000ff;
		if(0 == act->whdf.ix0)mcolor[5] = 0x0000ff;
	}

	//p: a, a', b, b'
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]/4 +vf[j]*3/4;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	gl41line_pmos(ctx, acolor, mcolor[0], tc,tr,tf,vt);
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]/4 +vf[j]*3/4;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	gl41line_pmos(ctx, xcolor, mcolor[1], tc,tr,tf,vt);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]/4 +vf[j]*1/4;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	gl41line_pmos(ctx, bcolor, mcolor[2], tc,tr,tf,vt);
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]/4 +vf[j]*1/4;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	gl41line_pmos(ctx, ycolor, mcolor[3], tc,tr,tf,vt);

	//n: a, a', b', b
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]/4 -vf[j]*1/4;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	gl41line_nmos(ctx, acolor, mcolor[4], tc,tr,tf,vt);
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]/4 -vf[j]*1/4;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	gl41line_nmos(ctx, xcolor, mcolor[5], tc,tr,tf,vt);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]/4 -vf[j]*3/4;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	gl41line_nmos(ctx, ycolor, mcolor[6], tc,tr,tf,vt);
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]/4 -vf[j]*3/4;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	gl41line_nmos(ctx, bcolor, mcolor[7], tc,tr,tf,vt);

	//a
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j] +vf[j]*3/4;
		tr[j] = tc[j] +vr[j]/2;
	}
	gl41line(ctx, acolor, tc,tr);

	//b
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j] +vf[j]*1/4;
		tr[j] = tc[j] +vr[j]/2;
	}
	gl41line(ctx, bcolor, tc,tr);

	//o
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j];
		tr[j] = tc[j] -vr[j]*5/4;
	}
	gl41line(ctx, ocolor, tc,tr);
}
static void xnor_draw_gl41(
	_obj* ent, struct style* slot,
	_obj* win, struct style* geom,
	_obj* ctx, struct style* area)
{
	int j;
	vec3 tc,tr,tf,tt;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
	gl41line_rect(ctx, 0x404040, vc, vr, vf);

	u32 acolor = ent->whdf.ix0 ? 0xff0000 : 0x0000ff;
	u32 bcolor = ent->whdf.iy0 ? 0xff0000 : 0x0000ff;
	u32 xcolor = ent->whdf.iy0 ? 0x0000ff : 0xff0000;
	u32 ocolor = ent->whdf.iz0 ? 0xff0000 : 0x0000ff;

	//b
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]*7/8 +vf[j]/4;
		tr[j] = tc[j] -vf[j];
	}
	gl41line(ctx, bcolor, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]*7/8 -vf[j]*3/4;
		tr[j] = tc[j] +vr[j]*6/4;
	}
	gl41line(ctx, bcolor, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vf[j]*3/4;
		tr[j] = tc[j] +vf[j]*1/4;
	}
	gl41line(ctx, bcolor, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]*5/8 -vf[j]*3/4;
		tr[j] = tc[j] +vf[j]*1/4;
	}
	gl41line(ctx, bcolor, tc,tr);

	//left
	u32 left_p = 0xffffff;
	u32 left_n = 0xffffff;
	if(ent->whdf.iy0)left_n = 0x0000ff;
	else left_p = 0xff0000;
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]*5/8 +vf[j]*1/4;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	gl41line_pmos(ctx, bcolor, left_p, tc,tr,tf,vt);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]*5/8 -vf[j]*1/4;
	}
	gl41line_nmos(ctx, bcolor, left_n, tc,tr,tf,vt);

	//not b
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]*5/8;
		tr[j] = tc[j] +vr[j]/8;
	}
	gl41line(ctx, xcolor, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]/2;
		tr[j] = tc[j] +vf[j]/2;
	}
	gl41line(ctx, xcolor, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]/2 +vf[j]/2;
		tr[j] = tc[j] +vr[j]*9/8;
	}
	gl41line(ctx, xcolor, tc,tr);

	//tg
	u32 tg_p = 0xffffff;
	u32 tg_n = 0xffffff;
	if(ent->whdf.iy0){
		if(ent->whdf.ix0)tg_p = tg_n = 0xff0000;
		else tg_p = tg_n = 0x0000ff;
	}
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vf[j]/4;
		tr[j] =-vf[j]/4;
		tf[j] = vr[j]/4;
	}
	gl41line_pmos(ctx, xcolor, tg_p, tc,tr,tf,vt);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vf[j]/4;
		tr[j] = vf[j]/4;
		tf[j] =-vr[j]/4;
	}
	gl41line_nmos(ctx, bcolor, tg_n, tc,tr,tf,vt);

	//a
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]/4 -vf[j]/4;
		tr[j] = tc[j] +vf[j]/2;
	}
	gl41line(ctx, acolor, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]*3/8 -vf[j]/4;
		tr[j] = tc[j] +vf[j]/2;
	}
	gl41line(ctx, acolor, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]/4;
		tr[j] = tc[j] +vr[j]/8;
	}
	gl41line(ctx, acolor, tc,tr);

	//right
	u32 right_p = 0xffffff;
	u32 right_n = 0xffffff;
	if(0 == ent->whdf.iy0){
		if(ent->whdf.ix0)right_n = 0x0000ff;
		else right_p = 0xff0000;
	}
	else{
		if(ent->whdf.ix0)right_n = 0xff0000;
		else right_p = 0x0000ff;
	}
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]*5/8 +vf[j]*1/4;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	gl41line_pmos(ctx, acolor, right_p, tc,tr,tf,vt);
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]*5/8 -vf[j]*1/4;
	}
	gl41line_nmos(ctx, acolor, right_n, tc,tr,tf,vt);

	//out
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]*5/8;
		tr[j] = tc[j] +vr[j]*3/8;
	}
	gl41line(ctx, ocolor, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]*6/8;
		tr[j] = tc[j] +vf[j]*3/4;
	}
	gl41line(ctx, ocolor, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]/4 +vf[j]*3/4;
		tr[j] = tc[j] +vr[j];
	}
	gl41line(ctx, ocolor, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]/4 -vf[j]/4;
		tr[j] = tc[j] +vf[j];
	}
	gl41line(ctx, ocolor, tc,tr);
}
static void xnor_draw_pixel(
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
static void xnor_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void xnor_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void xnor_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void xnor_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void xnor_read_byworld_bycam_bywnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	xnor_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void xnor_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
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
		xnor_read_byworld_bycam_bywnd(ent,foot, stack,sp);
		break;
	}
}
static void xnor_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, u8* buf,int len)
{
	u8 tmp;
	//logtoall("@xorgate_write:%x\n",buf[0]);
	if(_src_ == stack[sp-1].foottype){
		tmp = buf[0] - 0x30;
		if((tmp >= 0)&&(tmp <= 3)){
			ent->whdf.ix0 = (tmp>>0)&1;
			ent->whdf.iy0 = (tmp>>1)&1;
			ent->whdf.iz0 = !(ent->whdf.ix0 ^ ent->whdf.iy0);
		}
	}
}
static void xnor_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void xnor_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void xnor_search(_obj* act, u8* buf)
{
}
static void xnor_modify(_obj* act, u8* buf)
{
}
static void xnor_delete(_obj* act, u8* buf)
{
}
static void xnor_create(_obj* act, u8* buf)
{
	act->whdf.ix0 = random_read()&1;
	act->whdf.iy0 = random_read()&1;
	act->whdf.iz0 = !(act->whdf.ix0 ^ act->whdf.iy0);
}




void xnor_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex32('x','n','o','r');

	p->oncreate = (void*)xnor_create;
	p->ondelete = (void*)xnor_delete;
	p->onreader = (void*)xnor_search;
	p->onwriter = (void*)xnor_modify;

	p->onattach = (void*)xnor_attach;
	p->ondetach = (void*)xnor_detach;
	p->ontaking = (void*)xnor_taking;
	p->ongiving = (void*)xnor_giving;
}
