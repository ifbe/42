#include "libuser.h"
#define _karnaugh_ hex32('k','a','r','n')
void gl41data_convert(struct entity* wnd, struct style* area, struct event* ev, vec3 v);
void gl41data_before(struct entity* wnd);
void gl41data_after(struct entity* wnd);
void gl41data_tmpcam(struct entity* wnd);




static int truth_search(struct entity* act, u32 foot, struct halfrel* self[], struct halfrel* peer[])
{
	struct relation* rel;
	struct entity* world;

	rel = act->irel0;
	while(1){
		if(0 == rel)break;
		world = (void*)(rel->srcchip);
		if(_world3d_ == world->type){
			self[0] = (void*)&rel->dstchip;
			peer[0] = (void*)&rel->srcchip;
			return 1;
		}
		rel = samedstnextsrc(rel);
	}
	return 0;
}
static int truth_modify(struct entity* act)
{
	return 0;
}
static int truth_delete(struct entity* act)
{
	return 0;
}
static int truth_create(struct entity* act, u8* str)
{
	int j;
	u8* out = (void*)&act->data0;
	for(j=0;j<16;j++)out[j] = getrandom()&1;
	return 0;
}




void truth_draw_gl41(
	struct entity* act, struct style* part,
	struct entity* scn, struct style* geom,
	struct entity* wnd, struct style* area)
{
	u8* out;
	u8 ch[8];
	int x,y,j,rgb;
	vec3 tc,tr,tf;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vt = geom->f.vt;
	for(j=0;j<3;j++)tc[j] = vc[j] -vt[j]/100.0;
	gl41opaque_rect(wnd, 0x80800000, tc, vr, vf);

	out = (void*)&act->data0;
	for(j=0;j<3;j++){
		tr[j] = vr[j]/8.08;
		tf[j] = vf[j]/17.17;
	}
	for(y=0;y<16+1;y++){
		if(0 == y){
			ch[0] = 'A';
			ch[1] = 'B';
			ch[2] = 'C';
			ch[3] = 'D';
			ch[5] = 'Y';
		}
		else{
			ch[0] = '0' + (((y-1)>>3)&1);
			ch[1] = '0' + (((y-1)>>2)&1);
			ch[2] = '0' + (((y-1)>>1)&1);
			ch[3] = '0' + (((y-1)>>0)&1);
			ch[5] = '0' + out[y-1];
		}
		for(x=0;x<8;x++){
			for(j=0;j<3;j++)tc[j] = vc[j] +vr[j]*(x*2-7)/8 + vf[j]*(16-2*y)/17;
			if((x == act->ix0)&&(y == act->iy0))rgb = 0xff00ff;
			else rgb = 0x808080;
			gl41line_rect(wnd, rgb, tc, tr, tf);
			if(4 == x)continue;
			if(6 <= x)continue;
			carveascii_center(wnd, rgb, tc, tr, tf, ch[x]);
		}
	}
}
void truth_draw_pixel(struct entity* win, struct style* sty)
{
}
void truth_draw_html(struct entity* win, struct style* sty)
{
}
void truth_draw_tui(struct entity* win, struct style* sty)
{
}
void truth_draw_cli(struct entity* win, struct style* sty)
{
}




static void truth_read_bywnd(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
//wnd.area -> cam.gl41, cam.slot -> world.geom
	struct entity* wnd;struct style* area;
	struct entity* cam;struct style* gl41;
	wnd = peer->pchip;area = peer->pfoot;
	cam = self->pchip;gl41 = self->pfoot;

	struct fstyle fs;
	fs.vc[0] = 0.0;fs.vc[1] = 0.0;fs.vc[2] = 0.0;
	fs.vr[0] = 1.0;fs.vr[1] = 0.0;fs.vr[2] = 0.0;
	fs.vf[0] = 0.0;fs.vf[1] = 1.0;fs.vf[2] = 0.0;
	fs.vt[0] = 0.0;fs.vt[1] = 0.0;fs.vt[2] =-1.0;
	gl41data_before(wnd);
	truth_draw_gl41(cam, 0, 0,(void*)&fs, wnd,area);
	gl41data_tmpcam(wnd);
	gl41data_after(wnd);
}
static void truth_write_bywnd(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	struct entity* wnd;struct style* area;
	struct entity* ent;struct style* gl41;
	wnd = peer->pchip;area = peer->pfoot;
	ent = self->pchip;gl41 = self->pfoot;

	struct event* ev = buf;
	if('p' == (ev->what&0xff)){
		vec3 xyz;
		gl41data_convert(wnd, area, ev, xyz);
		ent->ix0 = (int)(8*xyz[0]);
		ent->iy0 = (int)(17*(1.0-xyz[1]));

		if(0x2b70 == ev->what){
			u8* out = (void*)&ent->data0;
			int x = ent->ix0;
			int y = ent->iy0;
			if((5 == x)&&(y > 0)&&(y <= 16))out[y-1] ^= 1;
			relationwrite(ent, _karnaugh_, 0, 0, out, 16);
		}
	}
}
static int truth_write_bykarnaugh(struct entity* ent, u8* i)
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




static int truth_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, u8* buf, int len)
{
	struct entity* ent = peer->pchip;
	switch(ent->fmt){
		case _gl41wnd0_:truth_read_bywnd(self, peer, stack, rsp, buf, len);break;
	}
	return 0;
}
static int truth_write(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, u8* buf, int len)
{
	struct entity* ent;
	struct entity* sup;

	ent = self->pchip;
	if(_karnaugh_ == self->flag){
		return truth_write_bykarnaugh(ent, buf);
	}

	sup = peer->pchip;
	switch(sup->fmt){
		case _gl41wnd0_:truth_write_bywnd(self, peer, stack, rsp, buf, len);break;
	}
	return 0;
}
static int truth_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
static int truth_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




void truth_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('t','r','u','t','h', 0, 0, 0);

	p->oncreate = (void*)truth_create;
	p->ondelete = (void*)truth_delete;
	p->onsearch = (void*)truth_search;
	p->onmodify = (void*)truth_modify;

	p->onlinkup = (void*)truth_linkup;
	p->ondiscon = (void*)truth_discon;
	p->onread  = (void*)truth_read;
	p->onwrite = (void*)truth_write;
}
