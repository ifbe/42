#include "libuser.h"
#define COUNT (0x100000/36)




static void field_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void field_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* wnd, struct style* area)
{
	int x,y,z;
	float tx,ty,tl;
	float ax,ay,az;
	float dx,dy,dz;
	float* vec;

	vec3 ta,tb;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vt = geom->f.vt;
	gl41line_prism4(wnd, 0xffffff, vc, vr, vf, vt);

	vec = act->buf0;
	for(z=0;z<20;z++){
	for(y=0;y<20;y++){
	for(x=0;x<20;x++){

		//field
		ax = x/10.0-0.95;
		ay = y/10.0-0.95;
		az = z/10.0-0.95;

		ta[0] = vc[0] + vr[0]*ax + vf[0]*ay + vt[0]*az;
		ta[1] = vc[1] + vr[1]*ax + vf[1]*ay + vt[1]*az;
		ta[2] = vc[2] + vr[2]*ax + vf[2]*ay + vt[2]*az;

		tx = ta[0] * (1000.0/squareroot(ta[0]*ta[0]+ta[1]*ta[1]) - 1.0);
		ty = ta[1] * (1000.0/squareroot(ta[0]*ta[0]+ta[1]*ta[1]) - 1.0);
		tl = squareroot(ta[2]*ta[2] + tx*tx + ty*ty) / 10000.0;
		dx = tx * tl;
		dy = ty * tl;
		dz =-ta[2] * tl;
		tb[0] = ta[0] + dx - ta[1]/1000;
		tb[1] = ta[1] + dy + ta[0]/1000;
		tb[2] = ta[2] + dz;

		gl41line_arrow(wnd, 0x007f7f, ta, tb, vt);


		//particle
		ta[0] = vec[(z*20*20 + y*20 + x)*3 + 0];
		ta[1] = vec[(z*20*20 + y*20 + x)*3 + 1];
		ta[2] = vec[(z*20*20 + y*20 + x)*3 + 2];

		tx = ta[0] * (1000.0/squareroot(ta[0]*ta[0]+ta[1]*ta[1]) - 1.0);
		ty = ta[1] * (1000.0/squareroot(ta[0]*ta[0]+ta[1]*ta[1]) - 1.0);
		tl = squareroot(ta[2]*ta[2] + tx*tx + ty*ty) / 100000.0;
		dx = tx * tl;
		dy = ty * tl;
		dz =-ta[2] * tl;
		vec[(z*20*20 + y*20 + x)*3 + 0] += dx - ta[1]/1000;
		vec[(z*20*20 + y*20 + x)*3 + 1] += dy + ta[0]/1000;
		vec[(z*20*20 + y*20 + x)*3 + 2] += dz;

		gl41point(wnd, 0xffffff, ta);
	}
	}
	}
}
static void field_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void field_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void field_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void field_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	say("field(%x,%x,%x)\n",win,act,sty);
}




static void field_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
//wnd -> cam, cam -> world
	struct entity* wnd;struct style* area;
	struct entity* wor;struct style* camg;

	//world -> video
	struct entity* scn;struct style* geom;
	struct entity* act;struct style* slot;

	if(stack){
		act = self->pchip;slot = self->pfoot;
		scn = peer->pchip;geom = peer->pfoot;
		wor = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		if('v' == len)field_draw_gl41(act,slot, scn,geom, wnd,area);
	}
}
static void field_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void field_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void field_linkup(struct halfrel* self, struct halfrel* peer)
{
	int x,y,z;
	float ax,ay,az;

	struct entity* act = (void*)(self->chip);
	float* vec = act->buf0;

	struct fstyle* sty = (void*)(peer->foot);
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vt;

	for(z=0;z<20;z++){
	for(y=0;y<20;y++){
	for(x=0;x<20;x++){
		ax = x/10.0-0.95;
		ay = y/10.0-0.95;
		az = z/10.0-0.95;

		vec[(z*20*20 + y*20 + x)*3 + 0] = vc[0] + vr[0]*ax + vf[0]*ay + vu[0]*az;
		vec[(z*20*20 + y*20 + x)*3 + 1] = vc[1] + vr[1]*ax + vf[1]*ay + vu[1]*az;
		vec[(z*20*20 + y*20 + x)*3 + 2] = vc[2] + vr[2]*ax + vf[2]*ay + vu[2]*az;
	}
	}
	}
}




static void field_search(struct entity* act)
{
}
static void field_modify(struct entity* act)
{
}
static void field_delete(struct entity* act)
{
	if(act->buf0){
		memorydelete(act->buf0);
		act->buf0 = 0;
	}
}
static void field_create(struct entity* act)
{
	float* buf = act->buf0 = memorycreate(0x100000, 0);
}




void field_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('f', 'i', 'e', 'l', 'd', 0, 0, 0);

	p->oncreate = (void*)field_create;
	p->ondelete = (void*)field_delete;
	p->onsearch = (void*)field_search;
	p->onmodify = (void*)field_modify;

	p->onlinkup = (void*)field_linkup;
	p->ondiscon = (void*)field_discon;
	p->onread  = (void*)field_read;
	p->onwrite = (void*)field_write;
}
