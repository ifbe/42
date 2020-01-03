#include "libuser.h"
void gl41data_convert(struct entity* wnd, struct style* area, struct event* ev, vec3 v);
void gl41data_before(struct entity* wnd);
void gl41data_after(struct entity* wnd);
void gl41data_tmpcam(struct entity* wnd);




static int karnaugh_search(struct entity* act, u32 foot, struct halfrel* self[], struct halfrel* peer[])
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




void karnaugh_draw_vbo(
	struct entity* act, struct style* part,
	struct entity* scn, struct style* geom,
	struct entity* wnd, struct style* area)
{
	u8* out;
	u8 ch[4];
	char* str[4] = {"00", "01", "11", "10"};
	int x,y,j,rgb;
	vec3 tc,tr,tf;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vt = geom->f.vt;
	for(j=0;j<3;j++)tc[j] = vc[j] -vt[j]/100.0;
	carveopaque_rect(wnd, 0x80000080, tc, vr, vf);

	out = (void*)&act->data0;
	for(j=0;j<3;j++){
		tr[j] = vr[j]/5.1;
		tf[j] = vf[j]/5.1;
	}
	for(y=0;y<5;y++){
		if(y>0){
			ch[0] = '0' + out[(y-1)*4 +0];
			ch[1] = '0' + out[(y-1)*4 +1];
			ch[2] = '0' + out[(y-1)*4 +2];
			ch[3] = '0' + out[(y-1)*4 +3];
		}
		for(x=0;x<5;x++){
			for(j=0;j<3;j++)tc[j] = vc[j] +vr[j]*(2*x-4)/5 +vf[j]*(4-2*y)/5;
			if(x>0&&y>0){
				if((x == act->ix0)&&(y == act->iy0))rgb = 0xff00ff;
				else rgb = 0xffffff;
				carveline_rect(wnd, 0xff0000, tc, tr, tf);
				carveascii_center(wnd, rgb, tc, tr, tf, ch[x-1]);
			}
		}
	}

	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j] +vf[j];
		tr[j] = tc[j] +vr[j]*2/5 -vf[j]*2/5;
	}
	carveline(wnd, 0xff0000, tc,tr);

	for(j=0;j<3;j++){
		tr[j] = vr[j]/5;
		tf[j] = vf[j]/5;
	}
	for(j=0;j<3;j++)tc[j] = vc[j] -vr[j] +vf[j]*7/10;
	carveascii(wnd, 0xffffff, tc,tr,tf, 'A');
	for(j=0;j<3;j++)tc[j] = vc[j] -vr[j]*7/8 +vf[j]*6/10;
	carveascii(wnd, 0xffffff, tc,tr,tf, 'B');
	for(j=0;j<3;j++)tc[j] = vc[j] -vr[j]*7/8 +vf[j]*8/10;
	carveascii(wnd, 0xffffff, tc,tr,tf, 'C');
	for(j=0;j<3;j++)tc[j] = vc[j] -vr[j]*6/8 +vf[j]*7/10;
	carveascii(wnd, 0xffffff, tc,tr,tf, 'D');

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




static void karnaugh_read_bywnd(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
//wnd.area -> cam.gl41, cam.slot -> world.geom
	int ret;
	struct entity* wnd;struct style* area;
	struct entity* cam;struct style* gl41;
	wnd = peer->pchip;area = peer->pfoot;
	cam = self->pchip;gl41 = self->pfoot;

	ret = karnaugh_search(cam, 0, &stack[rsp+0], &stack[rsp+1]);
	if(ret > 0){
		struct entity* act;struct style* slot;
		struct entity* wrd;struct style* geom;
		act = stack[rsp+0]->pchip;slot = stack[rsp+0]->pfoot;
		wrd = stack[rsp+1]->pchip;geom = stack[rsp+1]->pfoot;
		karnaugh_draw_vbo(act, slot, wrd,geom, wnd,area);
    }
    else{
		struct fstyle fs;
		fs.vc[0] = 0.0;fs.vc[1] = 0.0;fs.vc[2] = 0.0;
		fs.vr[0] = 1.0;fs.vr[1] = 0.0;fs.vr[2] = 0.0;
		fs.vf[0] = 0.0;fs.vf[1] = 1.0;fs.vf[2] = 0.0;
		fs.vt[0] = 0.0;fs.vt[1] = 0.0;fs.vt[2] =-1.0;
		gl41data_before(wnd);
		karnaugh_draw_vbo(cam, 0, 0,(void*)&fs, wnd,area);
		gl41data_after(wnd);

		gl41data_tmpcam(wnd);
    }
}
static void karnaugh_write_bywnd(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	struct entity* wnd;struct style* area;
	struct entity* ent;struct style* gl41;
	wnd = peer->pchip;area = peer->pfoot;
	ent = self->pchip;gl41 = self->pfoot;

	struct event* ev = buf;
	if('p' == (ev->what&0xff)){
		vec3 xyz;
		gl41data_convert(wnd, area, ev, xyz);
		ent->ix0 = (int)(5*xyz[0]);
		ent->iy0 = (int)(5*(1.0-xyz[1]));

		if(0x2b70 == ev->what){
			u8* out = (void*)&ent->data0;
			int x = ent->ix0 -1;
			int y = ent->iy0 -1;
			if((x >= 0)&&(x <= 3)&&(y >= 0)&&(y <= 3))out[y*4+x] ^= 1;
		}
	}
}




static int karnaugh_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, u8* buf, int len)
{
	struct entity* ent = peer->pchip;
	switch(ent->fmt){
		case _gl41wnd0_:karnaugh_read_bywnd(self, peer, stack, rsp, buf, len);break;
	}
	return 0;
}
static int karnaugh_write(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, u8* buf, int len)
{
	struct entity* ent = peer->pchip;
	switch(ent->fmt){
		case _gl41wnd0_:karnaugh_write_bywnd(self, peer, stack, rsp, buf, len);break;
	}
	return 0;
}
static int karnaugh_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
static int karnaugh_start(struct halfrel* self, struct halfrel* peer)
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

	p->onstart = (void*)karnaugh_start;
	p->onstop  = (void*)karnaugh_stop;
	p->onread  = (void*)karnaugh_read;
	p->onwrite = (void*)karnaugh_write;
}
