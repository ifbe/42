#include "libuser.h"




static void tardis_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void tardis_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void tardis_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void tardis_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void tardis_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void tardis_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* scn, struct style* geom,
	struct entity* wnd, struct style* area)
{
	int j;
	int time;
	vec3 tc,tr,tf,tt;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vt = geom->f.vt;
	gl41solid_rect(wnd, 0, vc, vr, vf);

	for(j=0;j<3;j++){
		tc[j] = vc[j] + vt[j]/2;
		tr[j] = vr[j]/2;
		tf[j] = vf[j]/2;
		tt[j] = vt[j]/2;
	}
	time = (timeread()%1000000)*512/1000000;
	if(time>255)time = 511-time;
	gl41opaque_prism4(wnd, 0x0000ff|(time<<24), tc, tr, tf, tt);
}
static void tardis_read_bycam(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key)
{
	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	if(stack && ('v'==key)){
		slot = stack[sp-1].pfoot;
		wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		tardis_draw_gl41(ent,slot, wor,geom, wnd,area);
	}
}




void tardis_pcm(struct entity* ent, struct supply* sup)
{
	int j;
	struct pcmdata* pcm;
	if(0 == ent->buf0)return;
//say("@tardis_pcm\n");
	pcm = ent->buf0 + 44 - 0x10;
	pcm->fmt = hex32('s','1','6',0);
	pcm->chan = 1;
	pcm->rate = 44100;
	pcm->count = 65536;
	sup->pcmdata = pcm;
}




static void tardis_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct supply* wnd = stack[sp-2].pchip;
//say("fmt=%.8s\n", &sup->fmt);

	switch(wnd->fmt){
		case _pcm_:tardis_pcm(ent, wnd);break;
		default:tardis_read_bycam(ent,foot, stack,sp, arg,key);
	}
}
static void tardis_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void tardis_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void tardis_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void tardis_search(struct entity* act)
{
}
static void tardis_modify(struct entity* act)
{
}
static void tardis_delete(struct entity* act)
{
	if(0 == act)return;
	//if(_copy_ == act->type)memorydelete(act->buf);
}
static void tardis_create(struct entity* act, void* arg)
{
	if(0 == act)return;

	act->buf0 = memorycreate(0x100000, 0);
	if(0 == act->buf0)return;

	if(0 == arg)arg = "datafile/wav/tardis.wav";
	openreadclose(arg, 0, act->buf0, 0x100000);
}




void tardis_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('t', 'a', 'r', 'd', 'i', 's', 0, 0);

	p->oncreate = (void*)tardis_create;
	p->ondelete = (void*)tardis_delete;
	p->onsearch = (void*)tardis_search;
	p->onmodify = (void*)tardis_modify;

	p->onlinkup = (void*)tardis_linkup;
	p->ondiscon = (void*)tardis_discon;
	p->ontaking = (void*)tardis_taking;
	p->ongiving = (void*)tardis_giving;
}
