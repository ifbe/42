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




static void resistor_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* scn, struct style* geom,
	struct entity* wnd, struct style* area)
{
	int j;
	vec3 tc,tr,tf,tt;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;

	for(j=0;j<3;j++)tc[j] = vc[j];
	gl41solid_prism4(wnd, 0x808080, vc, vr, vf, vt);

	for(j=0;j<3;j++){
		tc[j] = vc[j]+vt[j]*1.1;
		tr[j] = vr[j]/2;
		tf[j] = vf[j]/2;
	}
	gl41float(wnd, 0xffffff, tc,tr,tf, act->fx0);
}
static void resistor_read_bycam(_ent* ent,void* slot, _syn* stack,int sp)
{
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;

	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	resistor_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void resistor_read_a(struct entity* ent, int key, struct wireindex* sts, int thisone)
{
	if('R' != key)return;

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
static void resistor_read_b(struct entity* ent, int key, struct wireindex* sts, int thisone)
{
	if('R' != key)return;

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




static void resistor_taking(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	//foot defined behavior
	switch(stack[sp-1].flag){
	case 'a':
		resistor_read_a(ent, key, buf,len);
		return;
	case 'b':
		resistor_read_b(ent, key, buf,len);
		return;
	}

	//caller defined behavior
	struct entity* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(caller->fmt){
	case _rgba_:
		break;
	case _gl41full_:
		break;
	default:
		resistor_read_bycam(ent,foot, stack,sp);break;
	}
}
static void resistor_giving(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct wireindex* sts = buf;
	say("@resistor_write: %.4s\n", &foot);
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
	p->ontaking = (void*)resistor_taking;
	p->ongiving = (void*)resistor_giving;
}
