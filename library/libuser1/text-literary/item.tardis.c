#include "libuser.h"




static void tardis_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void tardis_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void tardis_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void tardis_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void tardis_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void tardis_draw_gl41(
	_obj* act, struct style* slot,
	_obj* scn, struct style* geom,
	_obj* wnd, struct style* area)
{
	int j;
	int time;
	vec3 tc,tr,tf,tt;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
	gl41solid_rect(wnd, 0, vc, vr, vf);

	for(j=0;j<3;j++){
		tc[j] = vc[j] + vt[j]/2;
		tr[j] = vr[j]/2;
		tf[j] = vf[j]/2;
		tt[j] = vt[j]/2;
	}
	time = (timeread_us()%1000000)*512/1000000;
	if(time>255)time = 511-time;
	gl41opaque_prism4(wnd, 0x0000ff|(time<<24), tc, tr, tf, tt);
}




void tardis_pcm(_obj* ent,void* slot, _obj* sup,void* area)
{
	int j;
	struct pcmdata* pcm;
	if(0 == ent->listptr.buf0)return;
//logtoall("@tardis_pcm\n");
	pcm = ent->listptr.buf0 + 44 - 0x10;
	pcm->fmt = hex32('s','1','6',0);
	pcm->chan = 1;
	pcm->rate = 44100;
	pcm->count = 65536;
	sup->pcmeasy.data = pcm;
}




static void tardis_read_byworld_bycam_bywnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	tardis_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void tardis_taking(_obj* ent,void* slot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	//foot defined behavior
	switch(stack[sp-1].foottype){
	}

	//caller defined behavior
	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(caller->type){
	case _pcm_:
		tardis_pcm(ent,slot, caller,area);
		break;
	case _wnd_:
		break;
	default:
		tardis_read_byworld_bycam_bywnd(ent,slot, stack,sp);
		break;
	}
}
static void tardis_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void tardis_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void tardis_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void tardis_search(_obj* act)
{
}
static void tardis_modify(_obj* act)
{
}
static void tardis_delete(_obj* act)
{
	if(0 == act)return;
	//if(_copy_ == act->type)memoryfree(act->buf);
}
static void tardis_create(_obj* act, void* arg)
{
	if(0 == act)return;

	act->listptr.buf0 = memoryalloc(0x100000, 0);
	if(0 == act->listptr.buf0)return;

	if(0 == arg)arg = "datafile/wav/tardis.wav";
	openreadclose(arg, 0, act->listptr.buf0, 0x100000);
}




void tardis_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex64('t', 'a', 'r', 'd', 'i', 's', 0, 0);

	p->oncreate = (void*)tardis_create;
	p->ondelete = (void*)tardis_delete;
	p->onreader = (void*)tardis_search;
	p->onwriter = (void*)tardis_modify;

	p->onattach = (void*)tardis_attach;
	p->ondetach = (void*)tardis_detach;
	p->ontaking = (void*)tardis_taking;
	p->ongiving = (void*)tardis_giving;
}
