#include "libuser.h"
#define LISTBUF buf0
void gl41data_before(struct entity* wnd);
void gl41data_after(struct entity* wnd);
void gl41data_01cam(struct entity* wnd);
void gl41data_convert(struct entity* wnd, struct style* area, struct event* ev, vec3 v);




static int slider_search(struct entity* act, u32 foot, struct halfrel* self[], struct halfrel* peer[])
{
	struct relation* rel;
	struct entity* world;

	rel = act->irel0;
	while(1){
		if(0 == rel)break;
		world = (void*)(rel->srcchip);
		if(_virtual_ == world->type){
			self[0] = (void*)&rel->dstchip;
			peer[0] = (void*)&rel->srcchip;
			return 1;
		}
		rel = samedstnextsrc(rel);
	}
	return 0;
}
static int slider_modify(struct entity* act)
{
	return 0;
}
static int slider_delete(struct entity* act)
{
	return 0;
}
static int slider_create(struct entity* act, u8* str)
{
	int j;
	int* list = act->LISTBUF = memorycreate(0x1000, 0);
	for(j=0;j<12;j++)list[j] = 50;

	act->iw0 = -1;
	return 0;
}





void slider_draw_gl41(
	struct entity* act, struct style* part,
	struct entity* scn, struct style* geom,
	struct entity* wnd, struct style* area)
{
	int x,y,j;
	int c,rgb;
	vec3 tc,tr,tf;
	int* list;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vt = geom->f.vt;
	gl41opaque_rect(wnd, 0x80404040, vc, vr, vf);

	list = act->LISTBUF;
	for(y=0;y<12;y++){
		for(j=0;j<3;j++){
			tc[j] = vc[j] +vf[j]*(5.5-y)/6 - vt[j]/100.0;
			tr[j] = vr[j] * 0.8;
			tf[j] = vf[j] / 80;
		}
		gl41solid_rect(wnd, 0, tc,tr,tf);

		for(j=0;j<3;j++){
			tc[j] = vc[j] +vr[j]*0.8*(list[y]-50)/50.0 +vf[j]*(5.5-y)/6;
			tr[j] = vr[j]/20;
			tf[j] = vf[j]/20;
		}
		gl41opaque_rect(wnd, 0x80808080, tc,tr,tf);
		carveascii_center(wnd, 0xffffff, tc,tr,tf, 0x30 + list[y]/10);
	}
}




static void slider_read_bywnd(_ent* ent,struct style* slot, _ent* wnd,struct style* area)
{
	struct fstyle fs;
	fs.vc[0] = 0.0;fs.vc[1] = 0.0;fs.vc[2] = 0.0;
	fs.vr[0] = 1.0;fs.vr[1] = 0.0;fs.vr[2] = 0.0;
	fs.vf[0] = 0.0;fs.vf[1] = 1.0;fs.vf[2] = 0.0;
	fs.vt[0] = 0.0;fs.vt[1] = 0.0;fs.vt[2] = 1.0;

	gl41data_before(wnd);
	slider_draw_gl41(ent, 0, 0,(void*)&fs, wnd,area);
	gl41data_01cam(wnd);
	gl41data_after(wnd);
}
static void slider_write_bywnd(_ent* ent,int foot, _syn* stack,int sp, struct event* ev,int len)
{
	struct entity* wnd;struct style* area;
	wnd = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	if('p' == (ev->what&0xff)){
		vec3 xyz;
		gl41data_convert(wnd, area, ev, xyz);

		int y = 12*(1.0-xyz[1]);
		if(0x2b70 == ev->what)ent->iw0 = y;
		if(0x2d70 == ev->what)ent->iw0 = -1;
		if(ent->iw0 < 0)return;

		int x = 120*xyz[0]-10;
		if(x < 0)x = 0;
		if(x > 100)x = 100;

		int* list = ent->LISTBUF;
		say("%d,%d\n", x, y);

		list[y] = x;
		give_data_into_peer(ent, _evto_, stack,sp, 0,0, list,12);
	}
}




static int slider_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	//struct entity* ent = stack[sp-1].pchip;
	struct style* slot = stack[sp-1].pfoot;
	struct entity* wnd = stack[sp-2].pchip;
	struct style* area = stack[sp-2].pfoot;

	switch(wnd->fmt){
	case _gl41wnd0_:
	case _full_:
	case _wnd_:{
		if('v' != key)break;
		slider_read_bywnd(ent,slot, wnd,area);break;
	}
	}
	return 0;
}
static int slider_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct supply* wnd = stack[sp-2].pchip;
	switch(wnd->fmt){
	case _gl41wnd0_:
	case _full_:
	case _wnd_:{
		slider_write_bywnd(ent,foot, stack,sp, buf,len);break;
	}
	}
	return 0;
}
static int slider_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
static int slider_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




void slider_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('s','l','i','d','e','r', 0, 0);

	p->oncreate = (void*)slider_create;
	p->ondelete = (void*)slider_delete;
	p->onsearch = (void*)slider_search;
	p->onmodify = (void*)slider_modify;

	p->onlinkup = (void*)slider_linkup;
	p->ondiscon = (void*)slider_discon;
	p->ontaking = (void*)slider_taking;
	p->ongiving = (void*)slider_giving;
}
