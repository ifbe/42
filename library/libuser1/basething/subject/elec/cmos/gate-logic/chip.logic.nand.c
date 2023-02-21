#include "libuser.h"
void gl41line_pmos(_obj* wnd, u32 irgb, u32 orgb, vec3 vc, vec3 vr, vec3 vf, vec3 vt);
void gl41line_nmos(_obj* wnd, u32 irgb, u32 orgb, vec3 vc, vec3 vr, vec3 vf, vec3 vt);




static void nand_draw_pixel(
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
static void nand_draw_gl41(
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
		tc[j] = vc[j] -vr[j]/2 +vf[j];
		tr[j] = tc[j] -vf[j]/4;
	}
	gl41line(ctx, 0xff0000, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]/2 +vf[j];
		tr[j] = tc[j] -vf[j]/4;
	}
	gl41line(ctx, 0xff0000, tc,tr);


	u8 pstatus[2];
	u8 nstatus[2];
	if(act->whdf.ix0){pstatus[0] = 0;nstatus[0] = 1;}
	else        {pstatus[0] = 1;nstatus[0] = 0;}
	if(act->whdf.iy0){pstatus[1] = 0;nstatus[1] = 1;}
	else        {pstatus[1] = 1;nstatus[1] = 0;}
	//say("%d,%d,%d,%d,%d,%d\n",act->whdf.ix0,act->whdf.iy0, pstatus[0],pstatus[1], nstatus[0],nstatus[1]);

	u32 xcolor = act->whdf.ix0 ? 0xff0000 : 0x0000ff;
	u32 ycolor = act->whdf.iy0 ? 0xff0000 : 0x0000ff;
	u32 ocolor = act->whdf.iz0 ? 0xff0000 : 0x0000ff;
	u32 pcolor[2] = {0xffffff, 0xffffff};
	u32 ncolor[2] = {0xffffff, 0xffffff};
	if(pstatus[0])pcolor[0] = 0xff0000;
	if(pstatus[1])pcolor[1] = 0xff0000;
	if(nstatus[1])ncolor[1] = 0x0000ff;
	if(nstatus[0]&&nstatus[1])ncolor[0] = 0x0000ff;

	//p1
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]/2 +vf[j]/2;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	gl41line_pmos(ctx, xcolor, pcolor[0], tc,tr,tf,vt);

	//p2
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]/2 +vf[j]/2;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	gl41line_pmos(ctx, ycolor, pcolor[1], tc,tr,tf,vt);

	//n1
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vf[j]*1/4;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	gl41line_nmos(ctx, xcolor, ncolor[0], tc,tr,tf,vt);

	//n2
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vf[j]*3/4;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	gl41line_nmos(ctx, ycolor, ncolor[1], tc,tr,tf,vt);

	//a
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j] -vf[j]*1/4;
		tr[j] = tc[j] +vr[j];
	}
	gl41line(ctx, xcolor, tc,tr);

	//b
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j] -vf[j]*3/4;
		tr[j] = tc[j] +vr[j];
	}
	gl41line(ctx, ycolor, tc,tr);

	//o
	for(j=0;j<3;j++){
		tc[j] = vc[j] - vr[j]/2;
		tr[j] = vc[j] + vr[j];
	}
	gl41line(ctx, ocolor, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]/2;
		tr[j] = tc[j] +vf[j]/4;
	}
	gl41line(ctx, ocolor, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]/2;
		tr[j] = tc[j] +vf[j]/4;
	}
	gl41line(ctx, ocolor, tc,tr);
}
static void nand_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void nand_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void nand_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void nand_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void nand_wrl_cam_wnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	nand_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void nand_taking(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
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
		nand_wrl_cam_wnd(ent,foot, stack,sp);
		break;
	}
}
static void nand_giving(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, u8* buf,int len)
{
	u8 tmp;
	say("@nandgate_write:%x\n",buf[0]);

	if(_src_ == stack[sp-1].foottype){
		tmp = buf[0] - 0x30;
		if((tmp >= 0)&&(tmp <= 3)){
			ent->whdf.ix0 = (tmp>>0)&1;
			ent->whdf.iy0 = (tmp>>1)&1;
			ent->whdf.iz0 = !(ent->whdf.ix0 && ent->whdf.iy0);
		}
	}
	else if('a' == stack[sp-1].foottype){
		if('0' == buf[0])ent->whdf.ix0 = 0;
		else if('1' == buf[0])ent->whdf.ix0 = 1;
		else return;
	}
	else if('b' == stack[sp-1].foottype){
		if('0' == buf[0])ent->whdf.iy0 = 0;
		else if('1' == buf[0])ent->whdf.iy0 = 1;
		else return;
	}
	else return;

	ent->whdf.iz0 = !(ent->whdf.ix0 && ent->whdf.iy0);
	tmp = ent->whdf.iz0 + 0x30;
	give_data_into_peer(ent,'o', stack,sp, 0,0, &tmp,1);
}
static void nand_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void nand_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void nand_search(_obj* act, u8* buf)
{
}
static void nand_modify(_obj* act, u8* buf)
{
}
static void nand_delete(_obj* act, u8* buf)
{
}
static void nand_create(_obj* act, u8* buf)
{
	act->whdf.ix0 = random_read()&1;
	act->whdf.iy0 = random_read()&1;
	act->whdf.iz0 = !(act->whdf.ix0 && act->whdf.iy0);
}




void nand_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex32('n','a','n','d');

	p->oncreate = (void*)nand_create;
	p->ondelete = (void*)nand_delete;
	p->onreader = (void*)nand_search;
	p->onwriter = (void*)nand_modify;

	p->onattach = (void*)nand_attach;
	p->ondetach = (void*)nand_detach;
	p->ontaking = (void*)nand_taking;
	p->ongiving = (void*)nand_giving;
}
