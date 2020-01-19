#include "libuser.h"
struct wireindex{
	int off;
	int cnt;
	float volt;
	float grad;
	u8 sure;
	u8 temp;
};




static void vsrc_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void vsrc_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* scn, struct style* geom,
	struct entity* wnd, struct style* area)
{
	int j;
	u64 time;
	vec3 tc,tr,tf,tt;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vt = geom->f.vt;

	//negative
	for(j=0;j<3;j++){
		tf[j] = vf[j]/16;
		tc[j] = vc[j] -vf[j]+tf[j];
	}
	gl41solid_cylinder(wnd, 0x404040, tc, vr, vt, tf);

	//positive
	for(j=0;j<3;j++){
		tf[j] = vf[j]/16;
		tc[j] = vc[j] +vf[j]-tf[j];
	}
	gl41solid_cylinder(wnd, 0xc00000, tc, vr, vt, tf);

	//middle
	gl41opaque_cylinder(wnd, 0x80808080, vc, vr, vt, vf);
	time = timeread()%1000000;
	for(j=0;j<3;j++)tc[j] = vc[j] + vf[j]*(time/500000.0 - 1.0);
	gl41solid_circle(wnd, (time/4000)<<16, tc,vr,vt);

	for(j=0;j<3;j++){
		tc[j] = vc[j]+vt[j]*1.1;
		tr[j] = vr[j]/2;
		tf[j] = vf[j]/2;
	}
	carvefloat(wnd, 0xffffff, tc,tr,tf, act->fx0);
}
static void vsrc_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void vsrc_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void vsrc_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void vsrc_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void vsrc_read_bycam(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
//wnd -> cam, cam -> world
	struct entity* wnd;struct style* area;
	struct entity* wrd;struct style* camg;
//world -> vsrc
	struct entity* win;struct style* geom;
	struct entity* act;struct style* slot;

	if(stack && ('v' == len)){
		act = self->pchip;slot = self->pfoot;
		win = peer->pchip;geom = peer->pfoot;
		wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		vsrc_draw_gl41(act,slot, win,geom, wnd,area);
	}
}




static void vsrc_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	switch(self->flag){
		case 'p':break;
		case 'n':break;
		default:vsrc_read_bycam(self, peer, stack,rsp, buf,len);break;
	}
}
static void vsrc_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	say("@vsrc_write: %.4s\n", &self->flag);
	if('n' == self->flag){
		struct entity* ent = self->pchip;
		struct wireindex* sts = buf;
		float volt = sts->volt + ent->fx0;
		relationwrite(ent, 'p', 0, 0, &volt, 0);
	}
}
static void vsrc_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void vsrc_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void vsrc_search(struct entity* act, u8* buf)
{
}
static void vsrc_modify(struct entity* act, u8* buf)
{
}
static void vsrc_delete(struct entity* act, u8* buf)
{
}
static void vsrc_create(struct entity* act, void* arg, int argc, u8** argv)
{
	if(0 == arg)return;
	decstr2float(arg, &act->fx0);
	say("V=%f\n",act->fx0);
}




void vsrc_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex32('v','s','r','c');

	p->oncreate = (void*)vsrc_create;
	p->ondelete = (void*)vsrc_delete;
	p->onsearch = (void*)vsrc_search;
	p->onmodify = (void*)vsrc_modify;

	p->onlinkup = (void*)vsrc_linkup;
	p->ondiscon = (void*)vsrc_discon;
	p->onread  = (void*)vsrc_read;
	p->onwrite = (void*)vsrc_write;
}
