#include "libuser.h"
#define P_PEERFOOT whdf.ixn
#define N_PEERFOOT whdf.iyn
struct wireindex{
	int off;
	int cnt;
	float volt;
	float grad;
	u8 sure;
	u8 temp;
};




static void vsrc_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void vsrc_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void vsrc_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void vsrc_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void vsrc_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void vsrc_draw_gl41(
	_obj* act, struct style* slot,
	_obj* scn, struct style* geom,
	_obj* wnd, struct style* area)
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
	time = timeread_us()%1000000;
	for(j=0;j<3;j++)tc[j] = vc[j] + vf[j]*(time/500000.0 - 1.0);
	gl41solid_circle(wnd, (time/4000)<<16, tc,vr,vt);

	for(j=0;j<3;j++){
		tc[j] = vc[j]+vt[j]*1.1;
		tr[j] = vr[j]/2;
		tf[j] = vf[j]/2;
	}
	gl41float(wnd, 0xffffff, tc,tr,tf, act->whdf.fx0);
}
static void vsrc_read_bycam(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	struct style* slot;
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	if(stack&&('v' == key)){
		slot = stack[sp-1].pfoot;
		wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		vsrc_draw_gl41(ent,slot, wor,geom, wnd,area);
	}
}




static void vsrc_read_p(_obj* ent, int key, struct wireindex* sts, int thisone)
{
	if('V' != key)return;

	int theother = ent->N_PEERFOOT - 'a';
	if(theother < 0)return;
	if(theother > 8)return;

	float vthat = sts[theother].volt;
	float vthis = sts[thisone].volt;
	say("@n: %d,%f, %d,%f\n",thisone,vthis, theother,vthat);

	float delta = vthis/2 - (vthat + ent->whdf.fx0)/2;
	say("vsrc_read_p: %f,%f\n", sts[thisone].grad, delta);
	sts[thisone].grad += delta;
}
static void vsrc_read_n(_obj* ent, int key, struct wireindex* sts, int thisone)
{
	if('V' != key)return;

	int theother = ent->P_PEERFOOT - 'a';
	if(theother < 0)return;
	if(theother > 8)return;

	float vthat = sts[theother].volt;
	float vthis = sts[thisone].volt;
	say("@n: %d,%f, %d,%f\n",thisone,vthis, theother,vthat);

	float delta = vthis/2 - (vthat - ent->whdf.fx0)/2;
	say("vsrc_read_n: %f,%f\n", sts[thisone].grad, delta);
	sts[thisone].grad += delta;
}




static void vsrc_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	switch(stack[sp-1].foottype){
		case 'p':vsrc_read_p(ent,key, buf,len);break;
		case 'n':vsrc_read_n(ent,key, buf,len);break;
		default:vsrc_read_bycam(ent,foot, stack,sp, arg,key, buf,len);break;
	}
}
static void vsrc_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	say("@vsrc_write: %.4s\n", &foot);
	if('n' == stack[sp-1].foottype){
		struct wireindex* sts = buf;
		float volt = sts->volt + ent->whdf.fx0;
		give_data_into_peer(ent,'p', stack,sp, 0,0, &volt,0);
	}
}
static void vsrc_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void vsrc_attach(struct halfrel* self, struct halfrel* peer)
{
	_obj* ent = self->pchip;
	switch(self->foottype){
		case 'p':ent->P_PEERFOOT = peer->foottype;break;
		case 'n':ent->N_PEERFOOT = peer->foottype;break;
	}
}




static void vsrc_search(_obj* act, u8* buf)
{
}
static void vsrc_modify(_obj* act, u8* buf)
{
}
static void vsrc_delete(_obj* act, u8* buf)
{
}
static void vsrc_create(_obj* act, void* arg, int argc, u8** argv)
{
	if(0 == arg)return;
	decstr2float(arg, &act->whdf.fx0);
	say("V=%f\n",act->whdf.fx0);
}




void vsrc_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex32('v','s','r','c');

	p->oncreate = (void*)vsrc_create;
	p->ondelete = (void*)vsrc_delete;
	p->onreader = (void*)vsrc_search;
	p->onwriter = (void*)vsrc_modify;

	p->onattach = (void*)vsrc_attach;
	p->ondetach = (void*)vsrc_detach;
	p->ontaking = (void*)vsrc_taking;
	p->ongiving = (void*)vsrc_giving;
}
