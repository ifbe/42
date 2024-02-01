#include "libuser.h"
void gl41line_pmos(_obj* wnd, u32 irgb, u32 orgb, vec3 vc, vec3 vr, vec3 vf, vec3 vt);
void gl41line_nmos(_obj* wnd, u32 irgb, u32 orgb, vec3 vc, vec3 vr, vec3 vf, vec3 vt);




static void mem_draw_pixel(
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
static void mem_draw_gl41(
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

	u32 lcolor[2] = {0xffffff, 0xffffff};
	u32 rcolor[2] = {0xffffff, 0xffffff};
	u32 gcolor[2];
	if(0 == act->whdf.ix0){lcolor[0] = 0xff0000;gcolor[0] = 0x0000ff;}
	else             {lcolor[1] = 0x0000ff;gcolor[0] = 0xff0000;}
	if(0 == act->whdf.iy0){rcolor[0] = 0xff0000;gcolor[1] = 0x0000ff;}
	else             {rcolor[1] = 0x0000ff;gcolor[1] = 0xff0000;}

	//l.p
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]/2 +vf[j]/2;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	gl41line_pmos(ctx, gcolor[0], lcolor[0], tc,tr,tf,vt);

	//l.n
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]/2 -vf[j]/2;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	gl41line_nmos(ctx, gcolor[0], lcolor[1], tc,tr,tf,vt);

	//l.g
	for(j=0;j<3;j++){
		tc[j] = vc[j] - vr[j]*3/4 - vf[j]/2;
		tr[j] = tc[j] + vf[j];
	}
	gl41line(ctx, gcolor[0], tc, tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] + vr[j]/2 - vf[j]/4;
		tr[j] = tc[j] + vf[j]/2;
	}
	gl41line(ctx, gcolor[0], tc, tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vf[j]/8 -vr[j]*3/4;
		tr[j] = vc[j] +vf[j]/8 +vr[j]/2;
	}
	gl41line(ctx, gcolor[0], tc, tr);

	//r.p
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]/2 +vf[j]/2;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	gl41line_pmos(ctx, gcolor[1], rcolor[0], tc,tr,tf,vt);

	//r.n
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]/2 -vf[j]/2;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	gl41line_nmos(ctx, gcolor[1], rcolor[1], tc,tr,tf,vt);

	//l.g
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]*1/4 -vf[j]/2;
		tr[j] = tc[j] +vf[j];
	}
	gl41line(ctx, gcolor[1], tc, tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]/2 -vf[j]/4;
		tr[j] = tc[j] +vf[j]/2;
	}
	gl41line(ctx, gcolor[1], tc, tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vf[j]/8 -vr[j]/2;
		tr[j] = vc[j] -vf[j]/8 +vr[j]*1/4;
	}
	gl41line(ctx, gcolor[1], tc, tr);
}
static void mem_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void mem_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void mem_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void mem_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void mem_read_byworld_bycam_bywnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	mem_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void mem_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
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
		mem_read_byworld_bycam_bywnd(ent,foot, stack,sp);
		break;
	}
}
static void mem_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, u8* buf,int len)
{
	u8 tmp;
	logtoall("@memgate_write:%x\n",buf[0]);

	if('0' == buf[0])ent->whdf.ix0 = 0;
	else if('1' == buf[0])ent->whdf.ix0 = 1;
	else return;

	ent->whdf.iy0 = !ent->whdf.ix0;
}
static void mem_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void mem_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void mem_search(_obj* act, u8* buf)
{
}
static void mem_modify(_obj* act, u8* buf)
{
}
static void mem_delete(_obj* act, u8* buf)
{
}
static void mem_create(_obj* act, u8* buf)
{
	act->whdf.ix0 = random_read()&1;
	act->whdf.iy0 = !act->whdf.ix0;
}




void mem_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex32('m','e','m', 0);

	p->oncreate = (void*)mem_create;
	p->ondelete = (void*)mem_delete;
	p->onreader = (void*)mem_search;
	p->onwriter = (void*)mem_modify;

	p->onattach = (void*)mem_attach;
	p->ondetach = (void*)mem_detach;
	p->ontaking = (void*)mem_taking;
	p->ongiving = (void*)mem_giving;
}
