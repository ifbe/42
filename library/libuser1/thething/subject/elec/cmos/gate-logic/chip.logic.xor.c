#include "libuser.h"
void gl41line_pmos(struct entity* wnd, u32 irgb, u32 orgb, vec3 vc, vec3 vr, vec3 vf, vec3 vt);
void gl41line_nmos(struct entity* wnd, u32 irgb, u32 orgb, vec3 vc, vec3 vr, vec3 vf, vec3 vt);




static void xor_draw_gl41_12t(
	struct entity* ent, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
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

	u32 acolor = ent->ix0 ? 0xff0000 : 0x0000ff;
	u32 xcolor = ent->ix0 ? 0x0000ff : 0xff0000;	//x=a'
	u32 bcolor = ent->ix0 ? 0xff0000 : 0x0000ff;
	u32 ycolor = ent->ix0 ? 0x0000ff : 0xff0000;	//y=b'
	u32 ocolor = ent->iz0 ? 0xff0000 : 0x0000ff;
	u32 mcolor[8] = {
		0xffffff, 0xffffff,
		0xffffff, 0xffffff,
		0xffffff, 0xffffff,
		0xffffff, 0xffffff
	};
	if(1 == ent->ix0){
		mcolor[0] = 0xff0000;
		if(0 == ent->ix0)mcolor[2] = 0xff0000;
	}
	if(0 == ent->ix0){
		mcolor[1] = 0xff0000;
		if(1 == ent->ix0)mcolor[3] = 0xff0000;
	}
	if(1 == ent->ix0){
		mcolor[6] = 0x0000ff;
		if(1 == ent->ix0)mcolor[4] = 0x0000ff;
	}
	if(0 == ent->ix0){
		mcolor[7] = 0x0000ff;
		if(0 == ent->ix0)mcolor[5] = 0x0000ff;
	}

	//p: a', a, b, b'
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]/4 +vf[j]*3/4;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	gl41line_pmos(ctx, xcolor, mcolor[0], tc,tr,tf,vt);
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]/4 +vf[j]*3/4;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	gl41line_pmos(ctx, acolor, mcolor[1], tc,tr,tf,vt);
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

	//n: a, a', b, b'
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
	gl41line_nmos(ctx, bcolor, mcolor[6], tc,tr,tf,vt);
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]/4 -vf[j]*3/4;
		tr[j] = vr[j]/4;
		tf[j] = vf[j]/4;
	}
	gl41line_nmos(ctx, ycolor, mcolor[7], tc,tr,tf,vt);

	//a
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j] -vf[j]*1/4;
		tr[j] = tc[j] +vr[j]/2;
	}
	gl41line(ctx, acolor, tc,tr);

	//b
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j] -vf[j]*3/4;
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
static void xor_draw_gl41(
	struct entity* ent, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	int j;
	vec3 tc,tr,tf,tt;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
	gl41line_rect(ctx, 0x404040, vc, vr, vf);

	u32 acolor = ent->iy0 ? 0xff0000 : 0x0000ff;
	u32 bcolor = ent->ix0 ? 0xff0000 : 0x0000ff;
	u32 xcolor = ent->ix0 ? 0x0000ff : 0xff0000;
	u32 ocolor = ent->iz0 ? 0xff0000 : 0x0000ff;

	//b
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]*7/8 -vf[j]/4;
		tr[j] = tc[j] +vf[j];
	}
	gl41line(ctx, bcolor, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]*7/8 +vf[j]*3/4;
		tr[j] = tc[j] +vr[j]*6/4;
	}
	gl41line(ctx, bcolor, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vf[j]*3/4;
		tr[j] = tc[j] -vf[j]*1/4;
	}
	gl41line(ctx, bcolor, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vr[j]*5/8 +vf[j]*3/4;
		tr[j] = tc[j] -vf[j]*1/4;
	}
	gl41line(ctx, bcolor, tc,tr);

	//left
	u32 left_p = 0xffffff;
	u32 left_n = 0xffffff;
	if(ent->ix0)left_n = 0x0000ff;
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
		tr[j] = tc[j] -vf[j]/2;
	}
	gl41line(ctx, xcolor, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]/2 -vf[j]/2;
		tr[j] = tc[j] +vr[j]*9/8;
	}
	gl41line(ctx, xcolor, tc,tr);

	//tg
	u32 tg_p = 0xffffff;
	u32 tg_n = 0xffffff;
	if(0 == ent->ix0){
		if(ent->iy0)tg_p = tg_n = 0xff0000;
		else tg_p = tg_n = 0x0000ff;
	}
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vf[j]/4;
		tr[j] =-vf[j]/4;
		tf[j] = vr[j]/4;
	}
	gl41line_pmos(ctx, bcolor, tg_p, tc,tr,tf,vt);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vf[j]/4;
		tr[j] = vf[j]/4;
		tf[j] =-vr[j]/4;
	}
	gl41line_nmos(ctx, xcolor, tg_n, tc,tr,tf,vt);

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
	if(ent->ix0){
		if(ent->iy0)right_n = 0x0000ff;
		else right_p = 0xff0000;
	}
	else{
		if(ent->iy0)right_n = 0xff0000;
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
		tr[j] = tc[j] -vf[j]*3/4;
	}
	gl41line(ctx, ocolor, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]/4 -vf[j]*3/4;
		tr[j] = tc[j] +vr[j];
	}
	gl41line(ctx, ocolor, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]/4 +vf[j]/4;
		tr[j] = tc[j] -vf[j];
	}
	gl41line(ctx, ocolor, tc,tr);
}
static void xor_draw_pixel(
	struct entity* ent, struct style* pin,
	struct entity* win, struct style* sty)
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
		cx = win->width/2;
		cy = win->height/2;
		ww = win->width/2;
		hh = win->height/2;
	}
}
static void xor_draw_json(
	struct entity* ent, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void xor_draw_html(
	struct entity* ent, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void xor_draw_tui(
	struct entity* ent, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void xor_draw_cli(
	struct entity* ent, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void xor_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	if(stack&&('v' == key)){
		slot = stack[sp-1].pfoot;
		wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		xor_draw_gl41(ent,slot, wor,geom, wnd,area);
	}
}
static void xor_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, u8* buf,int len)
{
	u8 tmp;
	if(_src_ == foot){
		tmp = buf[0] - 0x30;
		if((tmp >= 0)&&(tmp <= 3)){
			ent->ix0 = (tmp>>0)&1;
			ent->iy0 = (tmp>>1)&1;
			ent->iz0 = ent->ix0 ^ ent->iy0;
		}
	}
}
static void xor_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void xor_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void xor_search(struct entity* ent, u8* buf)
{
}
static void xor_modify(struct entity* ent, u8* buf)
{
}
static void xor_delete(struct entity* ent, u8* buf)
{
}
static void xor_create(struct entity* ent, u8* buf)
{
	ent->ix0 = getrandom()&1;
	ent->iy0 = getrandom()&1;
	ent->iz0 = ent->ix0 ^ ent->iy0;
}




void xor_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex32('x','o','r', 0);

	p->oncreate = (void*)xor_create;
	p->ondelete = (void*)xor_delete;
	p->onsearch = (void*)xor_search;
	p->onmodify = (void*)xor_modify;

	p->onlinkup = (void*)xor_linkup;
	p->ondiscon = (void*)xor_discon;
	p->ontaking = (void*)xor_taking;
	p->ongiving = (void*)xor_giving;
}
