#include "libuser.h"
#define P_PEERFOOT ixn
#define N_PEERFOOT iyn
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




static void vsrc_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* scn, struct style* geom,
	struct entity* wnd, struct style* area)
{
	int j;
	u64 time;
	vec3 tc,tr,tf,tt;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;

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
	gl41float(wnd, 0xffffff, tc,tr,tf, act->fx0);
}
static void vsrc_read_bycam(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	if(stack&&('v' == key)){
		slot = stack[sp-1].pfoot;
		wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		vsrc_draw_gl41(ent,slot, wor,geom, wnd,area);
	}
}




static void vsrc_read_p(struct entity* ent, int key, struct wireindex* sts, int thisone)
{
	if('V' != key)return;

	int theother = ent->N_PEERFOOT - 'a';
	if(theother < 0)return;
	if(theother > 8)return;

	float vthat = sts[theother].volt;
	float vthis = sts[thisone].volt;
	say("@n: %d,%f, %d,%f\n",thisone,vthis, theother,vthat);

	float delta = vthis/2 - (vthat + ent->fx0)/2;
	say("vsrc_read_p: %f,%f\n", sts[thisone].grad, delta);
	sts[thisone].grad += delta;
}
static void vsrc_read_n(struct entity* ent, int key, struct wireindex* sts, int thisone)
{
	if('V' != key)return;

	int theother = ent->P_PEERFOOT - 'a';
	if(theother < 0)return;
	if(theother > 8)return;

	float vthat = sts[theother].volt;
	float vthis = sts[thisone].volt;
	say("@n: %d,%f, %d,%f\n",thisone,vthis, theother,vthat);

	float delta = vthis/2 - (vthat - ent->fx0)/2;
	say("vsrc_read_n: %f,%f\n", sts[thisone].grad, delta);
	sts[thisone].grad += delta;
}




static void vsrc_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	switch(foot){
		case 'p':vsrc_read_p(ent,key, buf,len);break;
		case 'n':vsrc_read_n(ent,key, buf,len);break;
		default:vsrc_read_bycam(ent,foot, stack,sp, arg,key, buf,len);break;
	}
}
static void vsrc_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	say("@vsrc_write: %.4s\n", &foot);
	if('n' == foot){
		struct wireindex* sts = buf;
		float volt = sts->volt + ent->fx0;
		give_data_into_peer(ent,'p', stack,sp, 0,0, &volt,0);
	}
}
static void vsrc_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void vsrc_linkup(struct halfrel* self, struct halfrel* peer)
{
	struct entity* ent = self->pchip;
	switch(self->flag){
		case 'p':ent->P_PEERFOOT = peer->flag;break;
		case 'n':ent->N_PEERFOOT = peer->flag;break;
	}
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
	p->ontaking = (void*)vsrc_taking;
	p->ongiving = (void*)vsrc_giving;
}
