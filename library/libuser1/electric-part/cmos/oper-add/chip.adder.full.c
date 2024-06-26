#include "libuser.h"
void gl41line_pmos(_obj* wnd, u32 irgb, u32 orgb, vec3 vc, vec3 vr, vec3 vf, vec3 vt);
void gl41line_nmos(_obj* wnd, u32 irgb, u32 orgb, vec3 vc, vec3 vr, vec3 vf, vec3 vt);




static void fulladd_draw_gl41(
	_obj* act, struct style* slot,
	_obj* win, struct style* geom,
	_obj* ctx, struct style* area)
{
	int x,y,j;
	vec3 tc,tr,tf,tt;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
	gl41line_rect(ctx, 0x404040, vc, vr, vf);
/*
	//grid
	for(j=0;j<3;j++){
		tr[j] = vr[j]/8.1;
		tf[j] = vf[j]/8.1;
	}
	for(y=0;y<8;y++){
		for(x=0;x<8;x++){
			for(j=0;j<3;j++){
				tc[j] = vc[j] +vr[j]*(2*x-7)/8 +vf[j]*(2*y-7)/8;
			}
			gl41line_rect(ctx, 0x404040, tc,tr,tf);
		}
	}
*/
	//ia
	u32 iacolor = act->whdf.ix0 ? 0xff0000 : 0x0000ff;
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j] -vf[j]*2/8;
		tr[j] = tc[j] +vr[j]*3/4;
	}
	gl41line(ctx, iacolor, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]/4 -vf[j]*2/8;
		tr[j] = tc[j] -vf[j]*3/8;
	}
	gl41line(ctx, iacolor, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]*7/8 -vf[j]*2/8;
		tr[j] = tc[j] -vf[j]*5/8;
	}
	gl41line(ctx, iacolor, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]*7/8 -vf[j]*7/8;
		tr[j] = tc[j] +vr[j]*7/4;
	}
	gl41line(ctx, iacolor, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]*7/8 -vf[j]*7/8;
		tr[j] = tc[j] +vf[j]*3/8;
	}
	gl41line(ctx, iacolor, tc,tr);

	//ib
	u32 ibcolor = act->whdf.iy0 ? 0xff0000 : 0x0000ff;
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j] -vf[j]/8;
		tr[j] = tc[j] +vr[j]*7/8;
	}
	gl41line(ctx, ibcolor, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]*3/4 -vf[j]/8;
		tr[j] = tc[j] -vf[j]*4/8;
	}
	gl41line(ctx, ibcolor, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]*1/8 -vf[j]*1/8;
		tr[j] = tc[j] -vf[j]*1/8;
	}
	gl41line(ctx, ibcolor, tc,tr);

	//ic
	u32 iccolor = act->whdf.iz0 ? 0xff0000 : 0x0000ff;
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j] +vf[j]*6/8;
		tr[j] = tc[j] +vr[j]*6/8;
	}
	gl41line(ctx, iccolor, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]*1/4 +vf[j]*6/8;
		tr[j] = tc[j] -vf[j]*5/8;
	}
	gl41line(ctx, iccolor, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]*1/4 +vf[j]*1/8;
		tr[j] = tc[j] +vr[j]*9/8;
	}
	gl41line(ctx, iccolor, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]*7/8 +vf[j]*1/8;
		tr[j] = tc[j] -vf[j]*3/8;
	}
	gl41line(ctx, iccolor, tc,tr);

	//oc
	u32 occolor = act->whdf.iyn ? 0xff0000 : 0x0000ff;
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j] -vf[j]*4/8;
		tr[j] = tc[j] -vr[j]/8;
	}
	gl41line(ctx, occolor, tc,tr);

	//os
	u32 oscolor = act->whdf.ixn ? 0xff0000 : 0x0000ff;
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j] +vf[j]*4/8;
		tr[j] = tc[j] -vr[j]*5/8;
	}
	gl41line(ctx, oscolor, tc,tr);

	//bot,0
	u32 m0p = (0 == act->whdf.iy0) ? iacolor : 0xffffff;
	u32 m0n = (1 == act->whdf.iy0) ? 0x0000ff : 0xffffff;
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]*5/8 -vf[j]*3/8;
		tr[j] = vr[j]/8;
		tf[j] = vf[j]/8;
	}
	gl41line_pmos(ctx, ibcolor, m0p, tc,tr,tf,vt);
	for(j=0;j<3;j++)tc[j] = vc[j] -vr[j]*5/8 -vf[j]*5/8;
	gl41line_nmos(ctx, ibcolor, m0n, tc,tr,tf,vt);

	//mos0.o -> mos1.s
	u32 bot0to1 = 0xffffff;
	if(0xffffff != m0p)bot0to1 = m0p;
	if(0xffffff != m0n)bot0to1 = m0n;
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]*5/8 -vf[j]*4/8;
		tr[j] = tc[j] +vr[j]*4/8 -vf[j]*2/8;
	}
	gl41line(ctx, bot0to1, tc,tr);

	//bot,1
	u32 m1p = (0 == act->whdf.ix0) ? ibcolor : 0xffffff;
	u32 m1n = (1 == act->whdf.ix0) ? bot0to1 : 0xffffff;
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]*1/8 -vf[j]*3/8;
		tr[j] = vr[j]/8;
		tf[j] = vf[j]/8;
	}
	gl41line_pmos(ctx, iacolor, m1p, tc,tr,tf,vt);
	for(j=0;j<3;j++)tc[j] = vc[j] -vr[j]*1/8 -vf[j]*5/8;
	gl41line_nmos(ctx, iacolor, m1n, tc,tr,tf,vt);

	//mos1.o -> mos2.g
	u32 bot1to2 = 0xffffff;
	if(0xffffff != m1p)bot1to2 = m1p;
	if(0xffffff != m1n)bot1to2 = m1n;
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]*1/8 -vf[j]*4/8;
		tr[j] = tc[j] +vr[j]*3/8;
	}
	gl41line(ctx, bot1to2, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]*2/8 -vf[j]*3/8;
		tr[j] = tc[j] -vf[j]*2/8;
	}
	gl41line(ctx, bot1to2, tc,tr);

	//bot,2
	u32 m2p = (0x0000ff == bot1to2) ? 0xff0000 : 0xffffff;
	u32 m2n = (0xff0000 == bot1to2) ? 0x0000ff : 0xffffff;
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]*3/8 -vf[j]*3/8;
		tr[j] = vr[j]/8;
		tf[j] = vf[j]/8;
	}
	gl41line_pmos(ctx, bot1to2, m2p, tc,tr,tf,vt);
	for(j=0;j<3;j++)tc[j] = vc[j] +vr[j]*3/8 -vf[j]*5/8;
	gl41line_nmos(ctx, bot1to2, m2n, tc,tr,tf,vt);

	//mos2.o -> mos3.g
	u32 bot2to3 = 0xffffff;
	if(0xffffff != m2p)bot2to3 = m2p;
	if(0xffffff != m2n)bot2to3 = m2n;
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]*3/8 -vf[j]*4/8;
		tr[j] = tc[j] +vr[j]*3/8;
	}
	gl41line(ctx, bot2to3, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]*6/8 -vf[j]*3/8;
		tr[j] = tc[j] -vf[j]*2/8;
	}
	gl41line(ctx, bot2to3, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]/2;
		tr[j] = tc[j] -vf[j]/2;
	}
	gl41line(ctx, bot2to3, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]/2;
		tr[j] = tc[j] -vr[j]*5/4;
	}
	gl41line(ctx, bot2to3, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]*3/4;
		tr[j] = tc[j] +vf[j]*7/8;
	}
	gl41line(ctx, bot2to3, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]*3/4 +vf[j]*7/8;
		tr[j] = tc[j] +vr[j]*5/8;
	}
	gl41line(ctx, bot2to3, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]*1/8 +vf[j]*7/8;
		tr[j] = tc[j] -vf[j]*1/8;
	}
	gl41line(ctx, bot2to3, tc,tr);

	//bot,3
	u32 m3p = (0x0000ff == bot2to3) ? iccolor : 0xffffff;
	u32 m3n = (0xff0000 == bot2to3) ? iacolor : 0xffffff;
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]*7/8 -vf[j]*3/8;
		tr[j] = vr[j]/8;
		tf[j] = vf[j]/8;
	}
	gl41line_pmos(ctx, bot2to3, m3p, tc,tr,tf,vt);
	for(j=0;j<3;j++)tc[j] = vc[j] +vr[j]*7/8 -vf[j]*5/8;
	gl41line_nmos(ctx, bot2to3, m3n, tc,tr,tf,vt);

	//top,0
	u32 m4p = (0x0000ff == bot2to3) ? iccolor : 0xffffff;
	u32 m4n = (0xff0000 == bot2to3) ? 0x0000ff : 0xffffff;
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]*5/8 +vf[j]*5/8;
		tr[j] = vr[j]/8;
		tf[j] = vf[j]/8;
	}
	gl41line_pmos(ctx, bot2to3, m4p, tc,tr,tf,vt);
	for(j=0;j<3;j++)tc[j] = vc[j] -vr[j]*5/8 +vf[j]*3/8;
	gl41line_nmos(ctx, bot2to3, m4n, tc,tr,tf,vt);

	//top0.o -> top1.g
	u32 top0to1 = 0xffffff;
	if(0xffffff != m4p)top0to1 = m4p;
	if(0xffffff != m4n)top0to1 = m4n;
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]*5/8 +vf[j]*4/8;
		tr[j] = tc[j] +vr[j]*4/8 -vf[j]*2/8;
	}
	gl41line(ctx, top0to1, tc,tr);

	//top,1
	u32 m5p = (0x0000ff == iccolor) ? bot2to3 : 0xffffff;
	u32 m5n = (0xff0000 == iccolor) ? top0to1 : 0xffffff;
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]*1/8 +vf[j]*5/8;
		tr[j] = vr[j]/8;
		tf[j] = vf[j]/8;
	}
	gl41line_pmos(ctx, iccolor, m5p, tc,tr,tf,vt);
	for(j=0;j<3;j++)tc[j] = vc[j] -vr[j]*1/8 +vf[j]*3/8;
	gl41line_nmos(ctx, iccolor, m5n, tc,tr,tf,vt);

	//top1.o -> top2.g
	u32 top1to2 = 0xffffff;
	if(0xffffff != m5p)top1to2 = m5p;
	if(0xffffff != m5n)top1to2 = m5n;
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]*1/8 +vf[j]*4/8;
		tr[j] = tc[j] +vr[j]*3/8;
	}
	gl41line(ctx, top1to2, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]*2/8 +vf[j]*3/8;
		tr[j] = tc[j] +vf[j]*2/8;
	}
	gl41line(ctx, top1to2, tc,tr);

	//top,2
	u32 m6p = (0x0000ff == top1to2) ? 0xff0000 : 0xffffff;
	u32 m6n = (0xff0000 == top1to2) ? 0x0000ff : 0xffffff;
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]*3/8 +vf[j]*5/8;
		tr[j] = vr[j]/8;
		tf[j] = vf[j]/8;
	}
	gl41line_pmos(ctx, top1to2, m6p, tc,tr,tf,vt);
	for(j=0;j<3;j++)tc[j] = vc[j] +vr[j]*3/8 +vf[j]*3/8;
	gl41line_nmos(ctx, top1to2, m6n, tc,tr,tf,vt);

}




static void fulladd_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void fulladd_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void fulladd_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void fulladd_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void fulladd_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void fulladd_read_byworld_bycam_bywnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	fulladd_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void fulladd_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
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
		fulladd_read_byworld_bycam_bywnd(ent,foot, stack,sp);
		break;
	}
}
static void fulladd_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, u8* buf,int len)
{
	u8 x,y,z,tmp;
	//logtoall("@fulladd_write:%x\n",buf[0]);

	if(_src_ == stack[sp-1].foottype){
		tmp = buf[0] - 0x30;
		if((tmp >= 0)&&(tmp <= 7)){
			x = ent->whdf.ix0 = (tmp>>0)&1;
			y = ent->whdf.iy0 = (tmp>>1)&1;
			z = ent->whdf.iz0 = (tmp>>2)&1;

			ent->whdf.ixn = (x+y+z)&1;
			ent->whdf.iyn = (x+y+z)>>1;
		}
	}
}
static void fulladd_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void fulladd_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void fulladd_search(_obj* act, u8* buf)
{
}
static void fulladd_modify(_obj* act, u8* buf)
{
}
static void fulladd_delete(_obj* act, u8* buf)
{
}
static void fulladd_create(_obj* act, u8* buf)
{
	int x = act->whdf.ix0 = random_read()&1;
	int y = act->whdf.iy0 = random_read()&1;
	int z = act->whdf.iz0 = random_read()&1;

	act->whdf.ixn = (x+y+z)&1;
	act->whdf.iyn = (x+y+z)>>1;
}




void fulladd_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex64('f','u','l','l','a','d','d', 0);

	p->oncreate = (void*)fulladd_create;
	p->ondelete = (void*)fulladd_delete;
	p->onreader = (void*)fulladd_search;
	p->onwriter = (void*)fulladd_modify;

	p->onattach = (void*)fulladd_attach;
	p->ondetach = (void*)fulladd_detach;
	p->ontaking = (void*)fulladd_taking;
	p->ongiving = (void*)fulladd_giving;
}
