#include "libuser.h"
#define BLAST vfmt
#define DSTAMP listu64.data0
#define Dcount whdf.ix0
#define Dvalue whdf.ixn
#define SSTAMP listu64.data1
#define Scount whdf.iy0
#define Svalue whdf.iyn
#define BSTAMP listu64.data2
#define Bcount whdf.iz0
#define Bvalue whdf.izn
#define GSTAMP listu64.data3
#define Gcount whdf.iw0
#define Gvalue whdf.iwn
void gl41line_pmos(_obj* wnd, u32 irgb, u32 orgb, vec3 vc, vec3 vr, vec3 vf, vec3 vt);
void gl41line_nmos(_obj* wnd, u32 irgb, u32 orgb, vec3 vc, vec3 vr, vec3 vf, vec3 vt);




static void pmos_draw_pixel(
	_obj* ent, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void pmos_draw_json(
	_obj* ent, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void pmos_draw_html(
	_obj* ent, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void pmos_draw_tui(
	_obj* ent, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void pmos_draw_cli(
	_obj* ent, struct style* pin,
	_obj* win, struct style* sty)
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
	_obj* ent, struct style* slot,
	_obj* scn, struct style* geom,
	_obj* wnd, struct style* area)
{
	int j;
	vec3 tc,tr,tf;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;

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




static void pmos_read_D(_obj* mos,int key, struct halfrel* stack,int sp, u8* buf,int len)
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
static void pmos_write_D(_obj* mos,int key, struct halfrel* stack,int sp, u8* buf,int len)
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
static void pmos_read_S(_obj* mos,int key, struct halfrel* stack,int sp, u8* buf,int len)
{
}
static void pmos_write_S(_obj* mos,int key, struct halfrel* stack,int sp, u8* buf,int len)
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
static void pmos_read_G(_obj* mos,int key, struct halfrel* stack,int sp, u8* buf,int len)
{
}
static void pmos_write_G(_obj* mos,int key, struct halfrel* stack,int sp, u8* buf,int len)
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
			give_data_into_peer(mos,'D', stack,sp, 0,key, &tmp,1);
		}
	}
}




static void pmos_wrl_cam_wnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	pmos_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void pmos_taking(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	//foot defined behavior
	switch(stack[sp-1].foottype){
	case 'D':pmos_read_D(ent,key, stack,sp, buf,len);return;
	case 'S':pmos_read_S(ent,key, stack,sp, buf,len);return;
	//case 'B':pmos_read_B(ent,key, stack,sp, buf,len);return;
	case 'G':pmos_read_G(ent,key, stack,sp, buf,len);return;
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
		pmos_wrl_cam_wnd(ent,foot, stack,sp);
		break;
	}
}
static void pmos_giving(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, u8* buf,int len)
{
	say("pmos_write: %llx, %.4s=%x\n", ent, &foot, buf[0]);
	switch(stack[sp-1].foottype){
		case 'D':pmos_write_D(ent,key, stack,sp, buf,len);return;
		case 'S':pmos_write_S(ent,key, stack,sp, buf,len);return;
		//case 'B':pmos_write_B(ent,key, stack,sp, buf,len);return;
		case 'G':pmos_write_G(ent,key, stack,sp, buf,len);return;
	}
}
static void pmos_discon(struct halfrel* self, struct halfrel* peer)
{
	_obj* ent = self->pchip;
	switch(self->foottype){
		case 'D':ent->Dcount -= 1;return;
		case 'S':ent->Scount -= 1;return;
		//case 'B';ent->Bcount -= 1;return;
		case 'G':ent->Gcount -= 1;return;
	}
}
static void pmos_linkup(struct halfrel* self, struct halfrel* peer)
{
	_obj* ent = self->pchip;
	switch(self->foottype){
		case 'D':ent->Dcount += 1;return;
		case 'S':ent->Scount += 1;return;
		//case 'B';ent->Bcount += 1;return;
		case 'G':ent->Gcount += 1;return;
	}
}




static void pmos_search(_obj* ent, u8* buf)
{
}
static void pmos_modify(_obj* ent, u8* buf)
{
}
static void pmos_delete(_obj* ent, u8* buf)
{
}
static void pmos_create(_obj* ent, void* arg, int argc, u8** argv)
{
	ent->Dcount = ent->Scount = ent->Bcount = ent->Gcount = 0;
	ent->Gvalue = (getrandom()&0x2) -1;
	ent->Bvalue = 1;
	ent->Svalue = 1;
	ent->Dvalue = 0;
	if(-1 == ent->Gvalue)ent->Dvalue = 1;
}




void pmos_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex32('p','m','o','s');

	p->oncreate = (void*)pmos_create;
	p->ondelete = (void*)pmos_delete;
	p->onsearch = (void*)pmos_search;
	p->onmodify = (void*)pmos_modify;

	p->onlinkup = (void*)pmos_linkup;
	p->ondiscon = (void*)pmos_discon;
	p->ontaking = (void*)pmos_taking;
	p->ongiving = (void*)pmos_giving;
}
