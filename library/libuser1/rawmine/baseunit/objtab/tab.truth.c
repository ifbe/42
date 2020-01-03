#include "libuser.h"
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
	for(j=0;j<8;j++)out[j] = getrandom()&1;
	return 0;
}




void truth_draw_vbo(
	struct entity* act, struct style* part,
	struct entity* scn, struct style* geom,
	struct entity* wnd, struct style* area)
{
	u8* out;
	u8 ch[4];
	int x,y,j,rgb;
	vec3 tc,tr,tf;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vt = geom->f.vt;
	for(j=0;j<3;j++)tc[j] = vc[j] -vt[j]/100.0;
	carveopaque_rect(wnd, 0x80800000, tc, vr, vf);

	out = (void*)&act->data0;
	for(j=0;j<3;j++){
		tr[j] = vr[j]/4.04;
		tf[j] = vf[j]/9.09;
	}
	for(y=0;y<9;y++){
		if(0 == y){
			ch[0] = 'A';
			ch[1] = 'B';
			ch[2] = 'C';
			ch[3] = 'Y';
		}
		else{
			ch[0] = '0' + (((y-1)>>2)&1);
			ch[1] = '0' + (((y-1)>>1)&1);
			ch[2] = '0' + (((y-1)>>0)&1);
			ch[3] = '0' + out[y-1];
		}
		for(x=0;x<4;x++){
			for(j=0;j<3;j++)tc[j] = vc[j] +vr[j]*(x*2-3)/4 + vf[j]*(8-2*y)/9;
			if((x == act->ix0)&&(y == act->iy0))rgb = 0xff00ff;
			else rgb = 0x808080;
			carveline_rect(wnd, rgb, tc, tr, tf);
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
	int ret;
	struct entity* wnd;struct style* area;
	struct entity* cam;struct style* gl41;
	wnd = peer->pchip;area = peer->pfoot;
	cam = self->pchip;gl41 = self->pfoot;

	ret = truth_search(cam, 0, &stack[rsp+0], &stack[rsp+1]);
	if(ret > 0){
		struct entity* act;struct style* slot;
		struct entity* wrd;struct style* geom;
		act = stack[rsp+0]->pchip;slot = stack[rsp+0]->pfoot;
		wrd = stack[rsp+1]->pchip;geom = stack[rsp+1]->pfoot;
		truth_draw_vbo(act, slot, wrd,geom, wnd,area);
    }
    else{
		struct fstyle fs;
		fs.vc[0] = 0.0;fs.vc[1] = 0.0;fs.vc[2] = 0.0;
		fs.vr[0] = 1.0;fs.vr[1] = 0.0;fs.vr[2] = 0.0;
		fs.vf[0] = 0.0;fs.vf[1] = 1.0;fs.vf[2] = 0.0;
		fs.vt[0] = 0.0;fs.vt[1] = 0.0;fs.vt[2] =-1.0;
		gl41data_before(wnd);
		truth_draw_vbo(cam, 0, 0,(void*)&fs, wnd,area);
		gl41data_after(wnd);

		gl41data_tmpcam(wnd);
    }
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
		ent->ix0 = (int)(4*xyz[0]);
		ent->iy0 = (int)(9*(1.0-xyz[1]));

		if(0x2b70 == ev->what){
			u8* out = (void*)&ent->data0;
			int x = ent->ix0;
			int y = ent->iy0;
			if((x >= 3)&&(y > 0)&&(y < 9))out[y-1] ^= 1;
		}
	}
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
	struct entity* ent = peer->pchip;
	switch(ent->fmt){
		case _gl41wnd0_:truth_write_bywnd(self, peer, stack, rsp, buf, len);break;
	}
	return 0;
}
static int truth_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
static int truth_start(struct halfrel* self, struct halfrel* peer)
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

	p->onstart = (void*)truth_start;
	p->onstop  = (void*)truth_stop;
	p->onread  = (void*)truth_read;
	p->onwrite = (void*)truth_write;
}
