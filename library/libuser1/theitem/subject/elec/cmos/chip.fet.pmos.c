#include "libuser.h"
#define BLAST vfmt
#define DSTAMP data0
#define Dcount ix0
#define Dvalue ixn
#define SSTAMP data1
#define Scount iy0
#define Svalue iyn
#define BSTAMP data2
#define Bcount iz0
#define Bvalue izn
#define GSTAMP data3
#define Gcount iw0
#define Gvalue iwn
void gl41line_pmos(struct entity* wnd, u32 irgb, u32 orgb, vec3 vc, vec3 vr, vec3 vf, vec3 vt);
void gl41line_nmos(struct entity* wnd, u32 irgb, u32 orgb, vec3 vc, vec3 vr, vec3 vf, vec3 vt);




static void pmos_draw_pixel(
	struct entity* ent, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void pmos_draw_json(
	struct entity* ent, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void pmos_draw_html(
	struct entity* ent, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void pmos_draw_tui(
	struct entity* ent, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void pmos_draw_cli(
	struct entity* ent, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static u32 nmos_color(int val, int blast)
{
	if(blast)return 0xffff00;
	if(val < 0)return 0x0000ff;
	if(val > 0)return 0xff0000;
	return 0xffffff;
}
static void pmos_draw_gl41(
	struct entity* ent, struct style* slot,
	struct entity* scn, struct style* geom,
	struct entity* wnd, struct style* area)
{
	int j;
	vec3 tc,tr,tf;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vt = geom->f.vt;

	u32 dcolor = nmos_color(ent->Dvalue, ent->BLAST);
	u32 scolor = nmos_color(ent->Svalue, ent->BLAST);
	u32 bcolor = nmos_color(ent->Bvalue, ent->BLAST);
	u32 gcolor = nmos_color(ent->Gvalue, ent->BLAST);

	//source
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vf[j];
		tr[j] = tc[j] -vf[j]/2;
	}
	gl41line(wnd, scolor, tc, tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vf[j]/2;
		tr[j] = tc[j] -vr[j]/2;
	}
	gl41line(wnd, scolor, tc, tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vf[j]/2+vf[j]/8 -vr[j]/2;
		tr[j] = tc[j] -vf[j]/4;
	}
	gl41line(wnd, scolor, tc, tr);

	//drain
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vf[j];
		tr[j] = tc[j] +vf[j]/2;
	}
	gl41line(wnd, dcolor, tc, tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vf[j]/2;
		tr[j] = tc[j] -vr[j]/2;
	}
	gl41line(wnd, dcolor, tc, tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vf[j]/2-vf[j]/8 -vr[j]/2;
		tr[j] = tc[j] +vf[j]/4;
	}
	gl41line(wnd, dcolor, tc, tr);

	//base
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]/2;
	}
	gl41line(wnd, bcolor, tc, vc);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vf[j]/8 -vr[j]/2;
		tr[j] = tc[j] +vf[j]/4;
	}
	gl41line(wnd, bcolor, tc, tr);

	//gate
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j];
		tr[j] = vc[j] -vr[j]*9/16;
	}
	gl41line(wnd, gcolor, tc,tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j]*9/16 -vf[j]/2;
		tr[j] = tc[j] +vf[j];
	}
	gl41line(wnd, gcolor, tc, tr);
}
static void pmos_read_bycam(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, u8* buf,int len)
{
	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	if(stack&&('v' == key)){
		slot = stack[sp-1].pfoot;
		wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		pmos_draw_gl41(ent,slot, wor,geom, wnd,area);
	}
}




static void pmos_read_D(struct entity* mos,int key, struct halfrel* stack,int sp, u8* buf,int len)
{
	if(mos->Gvalue >= 0)return;

	if(0 == mos->Scount){
		buf[0] = 'p';
		return;
	}
	if((0 == mos->Dvalue)&&(0 != mos->Svalue)){
		mos->Dvalue = mos->Svalue;
		if(mos->Svalue > 0)buf[0] = 'p';
		if(mos->Dvalue < 0)buf[0] = 'n';
	}
}
static void pmos_write_D(struct entity* mos,int key, struct halfrel* stack,int sp, u8* buf,int len)
{
	//clear status
	if(('p' != buf[0])&&('n' != buf[0])){
		mos->Dvalue = 0;
		mos->BLAST = 0;
		return;
	}

	//check
	int D;
	if('p' == buf[0])D = 1;
	if('n' == buf[0])D = -1;
	if(key == mos->DSTAMP){
		if(D != mos->Dvalue)mos->BLAST = 1;
		return;
	}

	//set
	mos->DSTAMP = key;
	mos->Dvalue = D;
}
static void pmos_read_S(struct entity* mos,int key, struct halfrel* stack,int sp, u8* buf,int len)
{
}
static void pmos_write_S(struct entity* mos,int key, struct halfrel* stack,int sp, u8* buf,int len)
{
	//clear status
	if(('p' != buf[0])&&('n' != buf[0])){
		mos->Svalue = 0;
		mos->BLAST = 0;
		return;
	}

	//check
	int S;
	if('p' == buf[0])S = 1;
	if('n' == buf[0])S = -1;
	if(key == mos->SSTAMP){
		if(S != mos->Svalue)mos->BLAST = 1;
		return;
	}

	//set
	mos->SSTAMP = key;
	mos->Svalue = S;
}
static void pmos_read_G(struct entity* mos,int key, struct halfrel* stack,int sp, u8* buf,int len)
{
}
static void pmos_write_G(struct entity* mos,int key, struct halfrel* stack,int sp, u8* buf,int len)
{
	//clear status
	if(('p' != buf[0])&&('n' != buf[0])){
		mos->Gvalue = 0;
		mos->BLAST = 0;
		return;
	}

	//check
	int G;
	if('p' == buf[0])G = 1;
	if('n' == buf[0])G = -1;
	if(key == mos->GSTAMP){
		if(G != mos->Gvalue)mos->BLAST = 1;
		return;
	}

	//set
	mos->GSTAMP = key;
	mos->Gvalue = G;

	//chain
	if(0 == mos->Scount){
		if(mos->Gvalue < 0){
			mos->Dvalue = 1;

			u8 tmp = 'p';
			relationwrite(mos,'D', stack,sp, 0,key, &tmp,1);
		}
	}
}




static void pmos_read(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, u8* buf,int len)
{
	switch(foot){
		case 'D':pmos_read_D(ent,key, stack,sp, buf,len);return;
		case 'S':pmos_read_S(ent,key, stack,sp, buf,len);return;
		//case 'B':pmos_read_B(ent,key, stack,sp, buf,len);return;
		case 'G':pmos_read_G(ent,key, stack,sp, buf,len);return;
		default: pmos_read_bycam(ent,foot, stack,sp, arg,key, buf,len);
	}
}
static void pmos_write(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, u8* buf,int len)
{
	say("pmos_write: %llx, %.4s=%x\n", ent, &foot, buf[0]);
	switch(foot){
		case 'D':pmos_write_D(ent,key, stack,sp, buf,len);return;
		case 'S':pmos_write_S(ent,key, stack,sp, buf,len);return;
		//case 'B':pmos_write_B(ent,key, stack,sp, buf,len);return;
		case 'G':pmos_write_G(ent,key, stack,sp, buf,len);return;
	}
}
static void pmos_discon(struct halfrel* self, struct halfrel* peer)
{
	struct entity* ent = self->pchip;
	switch(self->flag){
		case 'D':ent->Dcount -= 1;return;
		case 'S':ent->Scount -= 1;return;
		//case 'B';ent->Bcount -= 1;return;
		case 'G':ent->Gcount -= 1;return;
	}
}
static void pmos_linkup(struct halfrel* self, struct halfrel* peer)
{
	struct entity* ent = self->pchip;
	switch(self->flag){
		case 'D':ent->Dcount += 1;return;
		case 'S':ent->Scount += 1;return;
		//case 'B';ent->Bcount += 1;return;
		case 'G':ent->Gcount += 1;return;
	}
}




static void pmos_search(struct entity* ent, u8* buf)
{
}
static void pmos_modify(struct entity* ent, u8* buf)
{
}
static void pmos_delete(struct entity* ent, u8* buf)
{
}
static void pmos_create(struct entity* ent, void* arg, int argc, u8** argv)
{
	ent->Dcount = ent->Scount = ent->Bcount = ent->Gcount = 0;
	ent->Gvalue = (getrandom()&0x2) -1;
	ent->Bvalue = 1;
	ent->Svalue = 1;
	ent->Dvalue = 0;
	if(-1 == ent->Gvalue)ent->Dvalue = 1;
}




void pmos_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex32('p','m','o','s');

	p->oncreate = (void*)pmos_create;
	p->ondelete = (void*)pmos_delete;
	p->onsearch = (void*)pmos_search;
	p->onmodify = (void*)pmos_modify;

	p->onlinkup = (void*)pmos_linkup;
	p->ondiscon = (void*)pmos_discon;
	p->onread  = (void*)pmos_read;
	p->onwrite = (void*)pmos_write;
}
