#include "libuser.h"
#define _truthtable_ hex32('t','r','u','e')
void gl41data_convert(struct entity* wnd, struct style* area, struct event* ev, vec3 v);
void gl41data_before(struct entity* wnd);
void gl41data_after(struct entity* wnd);
void gl41data_01cam(struct entity* wnd);




static int karnaugh_search(struct entity* act, u32 foot, struct halfrel* self[], struct halfrel* peer[])
{
	struct relation* rel;
	struct entity* world;

	rel = act->irel0;
	while(1){
		if(0 == rel)break;
		world = (void*)(rel->srcchip);
		if(_virtual_ == world->type){
			self[0] = (void*)&rel->dstchip;
			peer[0] = (void*)&rel->srcchip;
			return 1;
		}
		rel = samedstnextsrc(rel);
	}
	return 0;
}
static int karnaugh_modify(struct entity* act)
{
	return 0;
}
static int karnaugh_delete(struct entity* act)
{
	return 0;
}
static int karnaugh_create(struct entity* act, u8* str)
{
	int j;
	u8* out = (void*)&act->data0;
	for(j=0;j<16;j++)out[j] = getrandom()&1;
	return 0;
}




void karnaugh_draw_gl41_4x4(struct entity* wnd, struct entity* act, vec3 vc, vec3 vr, vec3 vf)
{
	int x,y,j,rgb;
	vec3 tc,tr,tf;
	u8 ch[4];
	u8* out = (void*)&act->data0;

	for(j=0;j<3;j++){
		tr[j] = vr[j]/4.1;
		tf[j] = vf[j]/4.1;
	}
	for(y=0;y<4;y++){
		ch[0] = '0' + out[y*4 +0];
		ch[1] = '0' + out[y*4 +1];
		ch[2] = '0' + out[y*4 +2];
		ch[3] = '0' + out[y*4 +3];
		for(x=0;x<4;x++){
			for(j=0;j<3;j++)tc[j] = vc[j] +vr[j]*(2*x-3)/4 +vf[j]*(3-2*y)/4;
			if((x == act->ix0)&&(y == act->iy0))rgb = 0xff00ff;
			else rgb = 0xffffff;
			gl41line_rect(wnd, 0xffffff, tc, tr, tf);
			gl41ascii_center(wnd, rgb, tc, tr, tf, ch[x]);
		}
	}

	//choose
	j = 0;
	for(j=0;j<16;j++){if(0 == out[j])break;}
	if(j == 16){
		for(j=0;j<3;j++){
			tr[j] = vr[j]*0.95;
			tf[j] = vf[j]*0.95;
		}
		gl41line_rectround(wnd, 0xff00ff, vc,tr,tf);
	}
}
void karnaugh_draw_gl41(
	struct entity* act, struct style* part,
	struct entity* scn, struct style* geom,
	struct entity* wnd, struct style* area)
{
	int x,y,j;
	vec3 tc,tr,tf;
	char* str[4] = {"00", "01", "11", "10"};
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
	for(j=0;j<3;j++)tc[j] = vc[j] -vt[j]/100.0;
	gl41opaque_rect(wnd, 0x80000080, tc, vr, vf);

	//frame
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j] +vf[j];
		tr[j] = tc[j] +vr[j]*2/5 -vf[j]*2/5;
	}
	gl41line(wnd, 0xff0000, tc,tr);

	for(j=0;j<3;j++){
		tr[j] = vr[j]/5;
		tf[j] = vf[j]/5;
	}
	for(j=0;j<3;j++)tc[j] = vc[j] -vr[j] +vf[j]*7/10;
	gl41ascii(wnd, 0xffffff, tc,tr,tf, 'A');
	for(j=0;j<3;j++)tc[j] = vc[j] -vr[j]*7/8 +vf[j]*6/10;
	gl41ascii(wnd, 0xffffff, tc,tr,tf, 'B');
	for(j=0;j<3;j++)tc[j] = vc[j] -vr[j]*7/8 +vf[j]*8/10;
	gl41ascii(wnd, 0xffffff, tc,tr,tf, 'C');
	for(j=0;j<3;j++)tc[j] = vc[j] -vr[j]*6/8 +vf[j]*7/10;
	gl41ascii(wnd, 0xffffff, tc,tr,tf, 'D');

	for(j=0;j<3;j++){
		tr[j] = vr[j]/10;
		tf[j] = vf[j]/10;
	}
	for(x=0;x<4;x++){
		for(j=0;j<3;j++)tc[j] = vc[j] +vr[j]*(2*x-2)/5 +vf[j]*4/5;
		carvestring_center(wnd, 0xffffff, tc, tr, tf, (u8*)str[x], 2);
	}
	for(y=0;y<4;y++){
		for(j=0;j<3;j++)tc[j] = vc[j] -vr[j]*4/5 +vf[j]*(2-2*y)/5;
		carvestring_center(wnd, 0xffffff, tc, tr, tf, (u8*)str[y], 2);
	}

	//4x4
	for(j=0;j<3;j++){
		tr[j] = vr[j]*4/5;
		tf[j] = vf[j]*4/5;
		tc[j] = vc[j] +vr[j]/5 -tf[j]/5;
	}
	karnaugh_draw_gl41_4x4(wnd, act, tc, tr, tf);
}
void karnaugh_draw_pixel(struct entity* win, struct style* sty)
{
}
void karnaugh_draw_html(struct entity* win, struct style* sty)
{
}
void karnaugh_draw_tui(struct entity* win, struct style* sty)
{
}
void karnaugh_draw_cli(struct entity* win, struct style* sty)
{
}




static void karnaugh_read_bywnd(_ent* ent,struct style* slot, _ent* wnd, struct style* area)
{
	struct fstyle fs;
	fs.vc[0] = 0.0;fs.vc[1] = 0.0;fs.vc[2] = 0.0;
	fs.vr[0] = 1.0;fs.vr[1] = 0.0;fs.vr[2] = 0.0;
	fs.vf[0] = 0.0;fs.vf[1] = 1.0;fs.vf[2] = 0.0;
	fs.vt[0] = 0.0;fs.vt[1] = 0.0;fs.vt[2] = 1.0;
	gl41data_before(wnd);
	karnaugh_draw_gl41(ent, 0, 0,(void*)&fs, wnd,area);
	gl41data_01cam(wnd);
	gl41data_after(wnd);
}
static void karnaugh_write_bywnd(_ent* ent,struct style* slot, _ent* wnd, struct style* area,
	_syn* stack,int sp, struct event* ev,int len)
{
	if('p' == (ev->what&0xff)){
		vec3 xyz;
		gl41data_convert(wnd, area, ev, xyz);
		ent->ix0 = (int)(5*xyz[0]) -1;
		ent->iy0 = (int)(5*(1.0-xyz[1])) -1;

		if(0x2b70 == ev->what){
			u8* out = (void*)&ent->data0;
			int x = ent->ix0;
			int y = ent->iy0;
			if((x >= 0)&&(x <= 3)&&(y >= 0)&&(y <= 3))out[y*4+x] ^= 1;
			give_data_into_peer(ent, _truthtable_, stack,sp, 0,0, out,16);
		}
	}
}
static int karnaugh_write_bytruthtable(struct entity* ent, u8* i)
{
	u8* o = (void*)&ent->data0;
	printmemory(i, 16);

	o[0] = i[0];
	o[1] = i[1];
	o[3] = i[2];
	o[2] = i[3];
	o[4] = i[4];
	o[5] = i[5];
	o[7] = i[6];
	o[6] = i[7];

	o[12] = i[8];
	o[13] = i[9];
	o[15] = i[10];
	o[14] = i[11];
	o[ 8] = i[12];
	o[ 9] = i[13];
	o[11] = i[14];
	o[10] = i[15];
	return 0;
}




static int karnaugh_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	//struct entity* ent = stack[sp-1].pchip;
	struct style* slot = stack[sp-1].pfoot;
	struct entity* wnd = stack[sp-2].pchip;
	struct style* area = stack[sp-2].pfoot;
	switch(wnd->fmt){
	case _gl41full_:{
		if('v' != key)break;
		karnaugh_read_bywnd(ent,slot, wnd,area);break;
	}
	}
	return 0;
}
static int karnaugh_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(_truthtable_ == foot){
		return karnaugh_write_bytruthtable(ent, buf);
	}

	//struct entity* ent = stack[sp-1].pchip;
	struct style* slot = stack[sp-1].pfoot;
	struct entity* wnd = stack[sp-2].pchip;
	struct style* area = stack[sp-2].pfoot;
	switch(wnd->fmt){
	case _gl41full_:karnaugh_write_bywnd(ent,slot, wnd,area, stack,sp, buf,len);break;
	}
	return 0;
}
static int karnaugh_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
static int karnaugh_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




void karnaugh_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('k','a','r','n','a','u','g','h');

	p->oncreate = (void*)karnaugh_create;
	p->ondelete = (void*)karnaugh_delete;
	p->onsearch = (void*)karnaugh_search;
	p->onmodify = (void*)karnaugh_modify;

	p->onlinkup = (void*)karnaugh_linkup;
	p->ondiscon = (void*)karnaugh_discon;
	p->ontaking = (void*)karnaugh_taking;
	p->ongiving = (void*)karnaugh_giving;
}
