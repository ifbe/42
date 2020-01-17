#include "libuser.h"
#define Dcount ix0
#define Dvalue ixn
#define Scount iy0
#define Svalue iyn
#define Bcount iz0
#define Bvalue izn
#define Gcount iw0
#define Gvalue iwn
void gl41line_pmos(struct entity* wnd, u32 irgb, u32 orgb, vec3 vc, vec3 vr, vec3 vf, vec3 vt);
void gl41line_nmos(struct entity* wnd, u32 irgb, u32 orgb, vec3 vc, vec3 vr, vec3 vf, vec3 vt);




static void nmos_draw_pixel(
	struct entity* ent, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void nmos_draw_json(
	struct entity* ent, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void nmos_draw_html(
	struct entity* ent, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void nmos_draw_tui(
	struct entity* ent, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void nmos_draw_cli(
	struct entity* ent, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static u32 nmos_color(int val)
{
	if(val < 0)return 0x0000ff;
	if(val > 0)return 0xff0000;
	return 0xffffff;
}
static void nmos_draw_gl41(
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
static void nmos_read_bycam(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, u8* buf, int len)
{
	struct entity* ent;struct style* slot;
	struct entity* scn;struct style* geom;
	struct entity* wrd;struct style* camg;
	struct entity* wnd;struct style* area;
	if(0 == stack)return;
	if('v' != len)return;

	ent = self->pchip;slot = self->pfoot;
	scn = peer->pchip;geom = peer->pfoot;
	wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;
	wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
	nmos_draw_gl41(ent,slot, scn,geom, wnd,area);
}




static void nmos_read_D(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, u8* buf, int len)
{
	struct entity* mos = self->pchip;
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
static void nmos_write_D(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, u8* buf, int len)
{
	struct entity* mos = self->pchip;
	switch(buf[0]){
		case 'p':mos->Dvalue = 1;break;
		case 'n':mos->Dvalue =-1;break;
		default:mos->Dvalue = 0;
	}
}
static void nmos_read_S(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, u8* buf, int len)
{
}
static void nmos_write_S(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, u8* buf, int len)
{
	struct entity* mos = self->pchip;
	switch(buf[0]){
		case 'p':mos->Svalue = 1;break;
		case 'n':mos->Svalue =-1;break;
		default:mos->Svalue = 0;
	}
}
static void nmos_read_G(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, u8* buf, int len)
{
}
static void nmos_write_G(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, u8* buf, int len)
{
	struct entity* mos = self->pchip;
	switch(buf[0]){
		case 'p':mos->Gvalue = 1;break;
		case 'n':mos->Gvalue =-1;break;
		default:mos->Gvalue = 0;
	}

	if(0 == mos->Scount){
		if(mos->Gvalue > 0){
			mos->Dvalue = -1;

			u8 tmp = 'n';
			relationwrite(mos, 'D', stack, rsp, &tmp, 1);
		}
	}
}




static void nmos_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, u8* buf, int len)
{
	switch(self->flag){
		case 'D':nmos_read_D(self,peer, stack,rsp, buf,len);return;
		case 'S':nmos_read_S(self,peer, stack,rsp, buf,len);return;
		//case 'B':nmos_read_B(self,peer, stack,rsp, buf,len);return;
		case 'G':nmos_read_G(self,peer, stack,rsp, buf,len);return;
		default: nmos_read_bycam(self,peer, stack,rsp, buf,len);
	}
}
static void nmos_write(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, u8* buf, int len)
{
	say("nmos_write: %.4s\n", &self->flag);
	switch(self->flag){
		case 'D':nmos_write_D(self,peer, stack,rsp, buf,len);return;
		case 'S':nmos_write_S(self,peer, stack,rsp, buf,len);return;
		//case 'B':nmos_write_B(self,peer, stack,rsp, buf,len);return;
		case 'G':nmos_write_G(self,peer, stack,rsp, buf,len);return;
	}
}
static void nmos_discon(struct halfrel* self, struct halfrel* peer)
{
	struct entity* ent = self->pchip;
	switch(self->flag){
		case 'D':ent->Dcount -= 1;return;
		case 'S':ent->Scount -= 1;return;
		//case 'B';ent->Bcount -= 1;return;
		case 'G':ent->Gcount -= 1;return;
	}
}
static void nmos_linkup(struct halfrel* self, struct halfrel* peer)
{
	struct entity* ent = self->pchip;
	switch(self->flag){
		case 'D':ent->Dcount += 1;return;
		case 'S':ent->Scount += 1;return;
		//case 'B';ent->Bcount += 1;return;
		case 'G':ent->Gcount += 1;return;
	}
}




static void nmos_search(struct entity* ent, u8* buf)
{
}
static void nmos_modify(struct entity* ent, u8* buf)
{
}
static void nmos_delete(struct entity* ent, u8* buf)
{
}
static void nmos_create(struct entity* ent, void* arg, int argc, u8** argv)
{
	ent->Dcount = ent->Scount = ent->Bcount = ent->Gcount = 0;
	ent->Gvalue = (getrandom()&0x2) -1;
	ent->Bvalue = -1;
	ent->Svalue = -1;
	ent->Dvalue = 0;
	if(1 == ent->Gvalue)ent->Dvalue = -1;
}




void nmos_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex32('n','m','o','s');

	p->oncreate = (void*)nmos_create;
	p->ondelete = (void*)nmos_delete;
	p->onsearch = (void*)nmos_search;
	p->onmodify = (void*)nmos_modify;

	p->onlinkup = (void*)nmos_linkup;
	p->ondiscon = (void*)nmos_discon;
	p->onread  = (void*)nmos_read;
	p->onwrite = (void*)nmos_write;
}
