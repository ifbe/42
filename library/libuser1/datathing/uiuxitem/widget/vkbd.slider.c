#include "libuser.h"
#define LISTBUF listptr.buf0
void dx11data_01cam(_obj* wnd);
//
void gl41data_before(_obj* wnd);
void gl41data_after(_obj* wnd);
void gl41data_01cam(_obj* wnd);
void gl41data_convert(_obj* wnd, struct style* area, struct event* ev, vec3 v);




static int slider_search(_obj* act, u32 foot, struct halfrel* self[], struct halfrel* peer[])
{
	return 0;
}
static int slider_modify(_obj* act)
{
	return 0;
}
static int slider_delete(_obj* act)
{
	return 0;
}
static int slider_create(_obj* act, u8* str)
{
	int j;
	int* list = act->LISTBUF = memorycreate(0x1000, 0);
	for(j=0;j<12;j++)list[j] = 50;

	act->whdf.iw0 = -1;
	return 0;
}





void slider_draw_gl41(
	_obj* act, struct style* part,
	_obj* scn, struct style* geom,
	_obj* wnd, struct style* area)
{
	int x,y,j;
	int c,rgb;
	vec3 tc,tr,tf;
	int* list;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
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
		gl41ascii_center(wnd, 0xffffff, tc,tr,tf, 0x30 + list[y]/10);
	}
}




static void slider_read_bywnd(_obj* ent,struct style* slot, _obj* wnd,struct style* area)
{
	struct fstyle fs;
	fs.vc[0] = 0.0;fs.vc[1] = 0.0;fs.vc[2] = 0.5;
	fs.vr[0] = 1.0;fs.vr[1] = 0.0;fs.vr[2] = 0.0;
	fs.vf[0] = 0.0;fs.vf[1] = 1.0;fs.vf[2] = 0.0;
	fs.vt[0] = 0.0;fs.vt[1] = 0.0;fs.vt[2] =-0.5;

	gl41data_before(wnd);
	slider_draw_gl41(ent, 0, 0,(void*)&fs, wnd,area);
	gl41data_after(wnd);

	if(_dx11list_ == wnd->hfmt)dx11data_nocam(wnd);
	else gl41data_nocam(wnd);
}
static void slider_write_bywnd(_obj* ent,void* foot, _syn* stack,int sp, struct event* ev,int len)
{
	_obj* wnd;struct style* area;
	wnd = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	if('p' == (ev->what&0xff)){
		vec3 xyz;
		gl41data_convert(wnd, area, ev, xyz);

		int y = 12*(1.0-xyz[1]);
		if(0x2b70 == ev->what)ent->whdf.iw0 = y;
		if(0x2d70 == ev->what)ent->whdf.iw0 = -1;
		if(ent->whdf.iw0 < 0)return;

		int x = 120*xyz[0]-10;
		if(x < 0)x = 0;
		if(x > 100)x = 100;

		int* list = ent->LISTBUF;
		say("%d,%d\n", x, y);

		list[y] = x;
		give_data_into_peer(ent, _evto_, stack,sp, 0,0, list,12);
	}
}




static int slider_taking(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	_obj* wnd = stack[sp-2].pchip;
	struct style* area = stack[sp-2].pfoot;

	switch(wnd->hfmt){
	case _rgba_:
		break;
	case _dx11list_:
	case _mt20list_:
	case _gl41list_:
	case _vk12list_:
		slider_read_bywnd(ent,foot, wnd,area);
		break;
	}
	return 0;
}
static int slider_giving(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	_obj* wnd = stack[sp-2].pchip;
	switch(wnd->hfmt){
	case _gl41list_:{
		slider_write_bywnd(ent,foot, stack,sp, buf,len);break;
	}
	}
	return 0;
}
static int slider_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
static int slider_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




void slider_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('s','l','i','d','e','r', 0, 0);

	p->oncreate = (void*)slider_create;
	p->ondelete = (void*)slider_delete;
	p->onreader = (void*)slider_search;
	p->onwriter = (void*)slider_modify;

	p->onattach = (void*)slider_attach;
	p->ondetach = (void*)slider_detach;
	p->ontaking = (void*)slider_taking;
	p->ongiving = (void*)slider_giving;
}
