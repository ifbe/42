#include "libuser.h"
#define A_PEERFOOT ixn
#define B_PEERFOOT iyn
struct wireindex{
	int off;
	int cnt;
	float volt;
	float grad;
	u8 sure;
	u8 temp;
};




static void resistor_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void resistor_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* scn, struct style* geom,
	struct entity* wnd, struct style* area)
{
	int j;
	vec3 tc,tr,tf,tt;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vt = geom->f.vt;

	for(j=0;j<3;j++)tc[j] = vc[j];
	gl41solid_prism4(wnd, 0x808080, vc, vr, vf, vt);

	for(j=0;j<3;j++){
		tc[j] = vc[j]+vt[j]*1.1;
		tr[j] = vr[j]/2;
		tf[j] = vf[j]/2;
	}
	carvefloat(wnd, 0xffffff, tc,tr,tf, act->fx0);
}
static void resistor_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void resistor_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void resistor_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void resistor_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void resistor_read_bycam(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
//wnd -> cam, cam -> world
	struct entity* wnd;struct style* area;
	struct entity* wrd;struct style* camg;
//world -> resistor
	struct entity* win;struct style* geom;
	struct entity* act;struct style* slot;

	if(stack && ('v' == len)){
		act = self->pchip;slot = self->pfoot;
		win = peer->pchip;geom = peer->pfoot;
		wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		resistor_draw_gl41(act,slot, win,geom, wnd,area);
	}
}
static void resistor_read_a(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, struct wireindex* sts, int thisone)
{
	if(rsp != 'R')return;

	struct entity* ent = self->pchip;
	int theother = ent->B_PEERFOOT - 'a';
	if(theother < 0)return;
	if(theother > 8)return;

	float vthat = sts[theother].volt;
	float vthis = sts[thisone].volt;
	say("@a: %d,%f, %d,%f\n",thisone,vthis, theother,vthat);

	float delta = (2*vthis - 2*vthat) / (ent->fx0);
	say("resistor_read_a: %f,%f\n", sts[thisone].grad, delta);
	sts[thisone].grad += delta;
}
static void resistor_read_b(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, struct wireindex* sts, int thisone)
{
	if(rsp != 'R')return;

	struct entity* ent = self->pchip;
	int theother = ent->A_PEERFOOT - 'a';
	if(theother < 0)return;
	if(theother > 8)return;

	float vthat = sts[theother].volt;
	float vthis = sts[thisone].volt;
	say("@b: %d,%f, %d,%f\n",thisone,vthis, theother,vthat);

	float delta = (2*vthis - 2*vthat) / (ent->fx0);
	say("resistor_read_b: %f,%f\n", sts[thisone].grad, delta);
	sts[thisone].grad += delta;
}




static void resistor_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	switch(self->flag){
		case 'a':resistor_read_a(self,peer, stack,rsp, buf,len);break;
		case 'b':resistor_read_b(self,peer, stack,rsp, buf,len);break;
		default:resistor_read_bycam(self, peer, stack,rsp, buf,len);break;
	}
}
static void resistor_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	struct wireindex* sts = buf;
	say("@resistor_write: %.4s\n", &self->flag);
	say("%d, %f\n", sts->sure, sts->volt);
}
static void resistor_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void resistor_linkup(struct halfrel* self, struct halfrel* peer)
{
	struct entity* ent = self->pchip;
	switch(self->flag){
		case 'a':ent->A_PEERFOOT = peer->flag;break;
		case 'b':ent->B_PEERFOOT = peer->flag;break;
	}
}




static void resistor_search(struct entity* act, u8* buf)
{
}
static void resistor_modify(struct entity* act, u8* buf)
{
}
static void resistor_delete(struct entity* act, u8* buf)
{
}
static void resistor_create(struct entity* act, void* arg, int argc, u8** argv)
{
	if(0 == arg)return;
	decstr2float(arg, &act->fx0);
	say("R=%f\n",act->fx0);
}




void resistor_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('r','e','s','i','s','t','o','r');

	p->oncreate = (void*)resistor_create;
	p->ondelete = (void*)resistor_delete;
	p->onsearch = (void*)resistor_search;
	p->onmodify = (void*)resistor_modify;

	p->onlinkup = (void*)resistor_linkup;
	p->ondiscon = (void*)resistor_discon;
	p->onread  = (void*)resistor_read;
	p->onwrite = (void*)resistor_write;
}
