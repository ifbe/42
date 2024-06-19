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




static void nmos_draw_pixel(
	_obj* ent, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void nmos_draw_json(
	_obj* ent, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void nmos_draw_html(
	_obj* ent, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void nmos_draw_tui(
	_obj* ent, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void nmos_draw_cli(
	_obj* ent, struct style* pin,
	_obj* win, struct style* sty)
{
}




static u32 nmos_color(int val)
{
	if(val < 0)return 0x0000ff;
	if(val > 0)return 0xff0000;
	return 0xffffff;
}
static void nmos_draw_gl41(
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
	//gl41line_rect(wnd, 0xffffff, vc, vr, vf);

	u32 dcolor = nmos_color(ent->Dvalue);
	u32 scolor = nmos_color(ent->Svalue);
	u32 bcolor = nmos_color(ent->Bvalue);
	u32 gcolor = nmos_color(ent->Gvalue);

	//drain
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vf[j];
		tr[j] = tc[j] -vf[j]/2;
	}
	gl41line(wnd, dcolor, tc, tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vf[j]/2;
		tr[j] = tc[j] -vr[j]/2;
	}
	gl41line(wnd, dcolor, tc, tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] +vf[j]/2+vf[j]/8 -vr[j]/2;
		tr[j] = tc[j] -vf[j]/4;
	}
	gl41line(wnd, dcolor, tc, tr);

	//source
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vf[j];
		tr[j] = tc[j] +vf[j]/2;
	}
	gl41line(wnd, scolor, tc, tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vf[j]/2;
		tr[j] = tc[j] -vr[j]/2;
	}
	gl41line(wnd, scolor, tc, tr);
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vf[j]/2-vf[j]/8 -vr[j]/2;
		tr[j] = tc[j] +vf[j]/4;
	}
	gl41line(wnd, scolor, tc, tr);

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




static void nmos_read_D(_obj* mos,int key, struct halfrel* stack,int sp, u8* buf,int len)
{
	if(mos->Gvalue <= 0)return;

	if(0 == mos->Scount){
		buf[0] = 'n';
		mos->Dvalue = 0;
		return;
	}
	if((0 == mos->Dvalue)&&(0 != mos->Svalue)){
		mos->Dvalue = mos->Svalue;
		if(mos->Svalue > 0)buf[0] = 'p';
		if(mos->Dvalue < 0)buf[0] = 'n';
	}
}
static void nmos_write_D(_obj* mos,int key, struct halfrel* stack,int sp, u8* buf,int len)
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
static void nmos_read_S(_obj* mos,int key, struct halfrel* stack,int sp, u8* buf,int len)
{
}
static void nmos_write_S(_obj* mos,int key, struct halfrel* stack,int sp, u8* buf,int len)
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
static void nmos_read_G(_obj* mos,int key, struct halfrel* stack,int sp, u8* buf,int len)
{
}
static void nmos_write_G(_obj* mos,int key, struct halfrel* stack,int sp, u8* buf,int len)
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
		if(mos->Gvalue > 0){
			mos->Dvalue = -1;

			u8 tmp = 'n';
			give_data_into_peer(mos,'D', stack,sp, 0,key, &tmp,1);
		}
	}
}




static void nmos_read_byworld_bycam_bywnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	nmos_draw_gl41(ent,slot, wor,geom, wnd,area);
}
static void nmos_read_byworld_bywnd(_obj* ent,void* foot, _syn* stack,int sp)
{
}
static void nmos_read_bywnd(_obj* ent,void* foot, _syn* stack,int sp)
{
	_obj* wnd;struct style* area;
	wnd = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	struct fstyle fs;
	fs.vc[0] = 0.0;fs.vc[1] = 0.0;fs.vc[2] = 0.0;
	fs.vr[0] = 1.0;fs.vr[1] = 0.0;fs.vr[2] = 0.0;
	fs.vf[0] = 0.0;fs.vf[1] = 1.0;fs.vf[2] = 0.0;
	fs.vt[0] = 0.0;fs.vt[1] = 0.0;fs.vt[2] = 1.0;

	gl41data_before(wnd);
	nmos_draw_gl41(ent, 0, 0,(void*)&fs, wnd,area);
	gl41data_01cam(wnd);
	gl41data_after(wnd);
}




static void nmos_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	//foot defined behavior
	switch(stack[sp-1].foottype){
	case 'D':
		nmos_read_D(ent,key, stack,sp, buf,len);
		return;
	case 'S':
		nmos_read_S(ent,key, stack,sp, buf,len);
		return;
	//case 'B':nmos_read_B(ent,key, stack,sp, buf,len);return;
	case 'G':
		nmos_read_G(ent,key, stack,sp, buf,len);
		return;
	}

	//caller defined behavior
	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(caller->type){
	case _wnd_:
	case _render_:
		nmos_read_bywnd(ent,foot, stack,sp);break;
	default:
		nmos_read_byworld_bycam_bywnd(ent,foot, stack,sp);break;
		break;
	}
}
static void nmos_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, u8* buf,int len)
{
	logtoall("nmos_write: %llx, %.4s=%x\n", ent, &foot, buf[0]);
	switch(stack[sp-1].foottype){
		case 'D':nmos_write_D(ent,key, stack,sp, buf,len);return;
		case 'S':nmos_write_S(ent,key, stack,sp, buf,len);return;
		//case 'B':nmos_write_B(ent,key, stack,sp, buf,len);return;
		case 'G':nmos_write_G(ent,key, stack,sp, buf,len);return;
	}
}
static void nmos_detach(struct halfrel* self, struct halfrel* peer)
{
	_obj* ent = self->pchip;
	switch(self->foottype){
		case 'D':ent->Dcount -= 1;return;
		case 'S':ent->Scount -= 1;return;
		//case 'B';ent->Bcount -= 1;return;
		case 'G':ent->Gcount -= 1;return;
	}
}
static void nmos_attach(struct halfrel* self, struct halfrel* peer)
{
	_obj* ent = self->pchip;
	switch(self->foottype){
		case 'D':ent->Dcount += 1;return;
		case 'S':ent->Scount += 1;return;
		//case 'B';ent->Bcount += 1;return;
		case 'G':ent->Gcount += 1;return;
	}
}




static void nmos_search(_obj* ent, u8* buf)
{
}
static void nmos_modify(_obj* ent, u8* buf)
{
}
static void nmos_delete(_obj* ent, u8* buf)
{
}
static void nmos_create(_obj* ent, void* arg, int argc, u8** argv)
{
	ent->Dcount = ent->Scount = ent->Bcount = ent->Gcount = 0;
	ent->Gvalue = (random_read()&0x2) -1;
	ent->Bvalue = -1;
	ent->Svalue = -1;
	ent->Dvalue = 0;
	if(1 == ent->Gvalue)ent->Dvalue = -1;
}




void nmos_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex32('n','m','o','s');

	p->oncreate = (void*)nmos_create;
	p->ondelete = (void*)nmos_delete;
	p->onreader = (void*)nmos_search;
	p->onwriter = (void*)nmos_modify;

	p->onattach = (void*)nmos_attach;
	p->ondetach = (void*)nmos_detach;
	p->ontaking = (void*)nmos_taking;
	p->ongiving = (void*)nmos_giving;
}
