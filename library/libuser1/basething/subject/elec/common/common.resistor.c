#include "libuser.h"
#define A_PEERFOOT whdf.ixn
#define B_PEERFOOT whdf.iyn
struct wireindex{
	int off;
	int cnt;
	float volt;
	float grad;
	u8 sure;
	u8 temp;
};




static void resistor_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void resistor_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void resistor_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void resistor_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void resistor_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void resistor_draw_gl41(
	_obj* act, struct style* slot,
	_obj* scn, struct style* geom,
	_obj* wnd, struct style* area)
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
	gl41float(wnd, 0xffffff, tc,tr,tf, act->whdf.fx0);
}
static void resistor_read_bycam(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;

	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	resistor_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void resistor_read_a(_obj* ent, int key, struct wireindex* sts, int thisone)
{
	if('R' != key)return;

	int theother = ent->B_PEERFOOT - 'a';
	if(theother < 0)return;
	if(theother > 8)return;

	float vthat = sts[theother].volt;
	float vthis = sts[thisone].volt;
	say("@a: %d,%f, %d,%f\n",thisone,vthis, theother,vthat);

	float delta = (2*vthis - 2*vthat) / (ent->whdf.fx0);
	say("resistor_read_a: %f,%f\n", sts[thisone].grad, delta);
	sts[thisone].grad += delta;
}
static void resistor_read_b(_obj* ent, int key, struct wireindex* sts, int thisone)
{
	if('R' != key)return;

	int theother = ent->A_PEERFOOT - 'a';
	if(theother < 0)return;
	if(theother > 8)return;

	float vthat = sts[theother].volt;
	float vthis = sts[thisone].volt;
	say("@b: %d,%f, %d,%f\n",thisone,vthis, theother,vthat);

	float delta = (2*vthis - 2*vthat) / (ent->whdf.fx0);
	say("resistor_read_b: %f,%f\n", sts[thisone].grad, delta);
	sts[thisone].grad += delta;
}




static void resistor_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	//foot defined behavior
	switch(stack[sp-1].foottype){
	case 'a':
		resistor_read_a(ent, key, buf,len);
		return;
	case 'b':
		resistor_read_b(ent, key, buf,len);
		return;
	}

	//caller defined behavior
	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(caller->hfmt){
	case _rgba_:
		break;
	case _gl41list_:
		break;
	default:
		resistor_read_bycam(ent,foot, stack,sp);break;
	}
}
static void resistor_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	struct wireindex* sts = buf;
	say("@resistor_write: %.4s\n", &foot);
	say("%d, %f\n", sts->sure, sts->volt);
}
static void resistor_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void resistor_attach(struct halfrel* self, struct halfrel* peer)
{
	_obj* ent = self->pchip;
	switch(self->foottype){
		case 'a':ent->A_PEERFOOT = peer->foottype;break;
		case 'b':ent->B_PEERFOOT = peer->foottype;break;
	}
}




static void resistor_search(_obj* act, u8* buf)
{
}
static void resistor_modify(_obj* act, u8* buf)
{
}
static void resistor_delete(_obj* act, u8* buf)
{
}
static void resistor_create(_obj* act, void* arg, int argc, u8** argv)
{
	if(0 == arg)return;
	decstr2float(arg, &act->whdf.fx0);
	say("R=%f\n",act->whdf.fx0);
}




void resistor_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('r','e','s','i','s','t','o','r');

	p->oncreate = (void*)resistor_create;
	p->ondelete = (void*)resistor_delete;
	p->onreader = (void*)resistor_search;
	p->onwriter = (void*)resistor_modify;

	p->onattach = (void*)resistor_attach;
	p->ondetach = (void*)resistor_detach;
	p->ontaking = (void*)resistor_taking;
	p->ongiving = (void*)resistor_giving;
}
