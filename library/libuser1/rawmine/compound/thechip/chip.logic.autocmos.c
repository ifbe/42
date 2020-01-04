#include "libuser.h"
void gl41data_before(struct entity* wnd);
void gl41data_after(struct entity* wnd);
void gl41data_tmpcam(struct entity* wnd);
void gl41data_convert(struct entity* wnd, struct style* area, struct event* ev, vec3 v);
void carveline_pmos(struct entity* wnd, u32 irgb, u32 orgb, vec3 vc, vec3 vr, vec3 vf, vec3 vt);
void carveline_nmos(struct entity* wnd, u32 irgb, u32 orgb, vec3 vc, vec3 vr, vec3 vf, vec3 vt);




static void autocmos_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	int cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->f.vc[0];
		cy = sty->f.vc[1];
		ww = sty->f.vr[0];
		hh = sty->f.vf[1];
	}
	else
	{
		cx = win->width/2;
		cy = win->height/2;
		ww = win->width/2;
		hh = win->height/2;
	}
}
static void autocmos_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void autocmos_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void autocmos_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void autocmos_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void autocmos_draw_vbo(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	int w,h;
	int x,y,j;
	vec3 tc,tr,tf,tt;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vt = geom->f.vt;
	carveopaque_rect(ctx, 0x40404040, vc, vr, vf);

	//vcc
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j] +vf[j];
		tr[j] = vc[j] +vr[j] +vf[j];
	}
	carveline(ctx, 0xff0000, tc, tr);

	//gnd
	for(j=0;j<3;j++){
		tc[j] = vc[j] -vr[j] -vf[j];
		tr[j] = vc[j] +vr[j] -vf[j];
	}
	carveline(ctx, 0x0000ff, tc, tr);

	//nmos
	w = act->ix0;
	h = act->iy0;
	for(j=0;j<3;j++){
		tr[j] = vr[j]/w;
		tf[j] = vf[j]/h/2;
	}
	for(y=0;y<h;y++){
		for(x=0;x<w;x++){
			for(j=0;j<3;j++){
				tc[j] = vc[j] +vr[j]*(x*2-w+1)/w -vf[j]+vf[j]*(2*y+1)/h/2;
			}
			carveline_nmos(ctx, 0xffffff, 0xffffff, tc,tr,tf,vt);
		}
	}

	//pmos
	w = act->ixn;
	h = act->iyn;
	for(j=0;j<3;j++){
		tr[j] = vr[j]/w;
		tf[j] = vf[j]/h/2;
	}
	for(y=0;y<h;y++){
		for(x=0;x<w;x++){
			for(j=0;j<3;j++){
				tc[j] = vc[j] +vr[j]*(x*2-w+1)/w +vf[j]-vf[j]*(2*y+1)/h/2;
			}
			carveline_pmos(ctx, 0xffffff, 0xffffff, tc,tr,tf,vt);
		}
	}
}
static void autocmos_read_bywnd(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
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
	autocmos_draw_vbo(cam, 0, 0,(void*)&fs, wnd,area);
	gl41data_after(wnd);

	gl41data_tmpcam(wnd);
}
static void autocmos_read_bycam(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, u8* buf, int len)
{
//wnd -> cam, cam -> world
	struct entity* wnd;struct style* area;
	struct entity* wrd;struct style* camg;
//world -> autocmos
	struct entity* win;struct style* geom;
	struct entity* act;struct style* slot;

	if(stack){
		act = self->pchip;slot = self->pfoot;
		win = peer->pchip;geom = peer->pfoot;
		wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		if('v' == len)autocmos_draw_vbo(act,slot, win,geom, wnd,area);
	}
}




static int autocmos_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, u8* buf, int len)
{
	struct entity* sup = peer->pchip;
	switch(sup->fmt){
		case _gl41wnd0_:autocmos_read_bywnd(self, peer, stack, rsp, buf, len);break;
		default:        autocmos_read_bycam(self, peer, stack, rsp, buf, len);break;
	}
	return 0;
}
static void autocmos_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
	struct entity* ent = self->pchip;
	//say("@autocmos_write:%x\n",buf[0]);
}
static void autocmos_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void autocmos_start(struct halfrel* self, struct halfrel* peer)
{
}




static void autocmos_search(struct entity* act, u8* buf)
{
}
static void autocmos_modify(struct entity* act, u8* buf)
{
}
static void autocmos_delete(struct entity* act, u8* buf)
{
}
static void autocmos_create(struct entity* act, u8* buf)
{
	act->ix0 = 3;
	act->iy0 = 4;
	act->ixn = 4;
	act->iyn = 3;
}




void autocmos_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('a','u','t','o','c','m','o','s');

	p->oncreate = (void*)autocmos_create;
	p->ondelete = (void*)autocmos_delete;
	p->onsearch = (void*)autocmos_search;
	p->onmodify = (void*)autocmos_modify;

	p->onstart = (void*)autocmos_start;
	p->onstop  = (void*)autocmos_stop;
	p->onread  = (void*)autocmos_read;
	p->onwrite = (void*)autocmos_write;
}
