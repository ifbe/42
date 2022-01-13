#include "libuser.h"
#define STRBUF buflen.buf
void dx11data_01cam(_obj* wnd);
//
void gl41data_before(_obj* wnd);
void gl41data_after(_obj* wnd);
void gl41data_01cam(_obj* wnd);
void gl41data_convert(_obj* wnd, struct style* area, struct event* ev, vec3 v);




void button_draw_pixel(
	_obj* act, struct style* part,
	_obj* ctx, struct style* area)
{
	int x,y;
	int time;
	int w = ctx->whdf.width;
	int h = ctx->whdf.height;
	u32* pix = ctx->rgbanode.buf;

	time = 511*(timeread()%1000000)/1000000;
	if(time > 255)time = 511-time;
	for(y=0;y<8;y++){
	for(x=0;x<8;x++){
		pix[y*w+x] = (0x010000*time) | (0x001f00*y) | (0x00001f*x);
	}
	}
}




void button_draw_gl41(
	_obj* act, struct style* part,
	_obj* win, struct style* geom,
	_obj* ctx, struct style* area)
{
	int j;
	vec3 tc;
	float* vc = geom->fshape.vc;
	float* vr = geom->fshape.vr;
	float* vf = geom->fshape.vf;
	float* vt = geom->fshape.vt;
	for(j=0;j<3;j++)tc[j] = vc[j]-vt[j]/64;
	gl41opaque_rect(ctx, 0x40ffd010, tc, vr, vf);
	gl41string_center(ctx, 0xff0000, vc, vr ,vf, act->STRBUF, 0);
}
static void button_read_bycam(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key)
{
	struct style* slot;
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	if(0 == stack)return;

	slot = stack[sp-1].pfoot;
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	button_draw_gl41(ent,slot, wor,geom, wnd,area);
}
static void button_read_bywnd(_obj* ent,struct style* slot, _obj* wnd,struct style* area)
{
	struct fstyle fs;
	fs.vc[0] = 0.0;fs.vc[1] = 0.0;fs.vc[2] = 0.5;
	fs.vr[0] = 1.0;fs.vr[1] = 0.0;fs.vr[2] = 0.0;
	fs.vf[0] = 0.0;fs.vf[1] = 1.0;fs.vf[2] = 0.0;
	fs.vt[0] = 0.0;fs.vt[1] = 0.0;fs.vt[2] =-0.5;

	gl41data_before(wnd);
	button_draw_gl41(ent, 0, 0,(void*)&fs, wnd,area);
	gl41data_after(wnd);

	if(_dx11list_ == wnd->hfmt)dx11data_nocam(wnd);
	else gl41data_nocam(wnd);
}




static void button_taking(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	_obj* wnd = stack[sp-2].pchip;
	struct style* area = stack[sp-2].pfoot;

	switch(wnd->hfmt){
	case _rgba_:
		button_draw_pixel(ent,foot, (void*)wnd,area);
		break;
	case _dx11list_:
	case _mt20list_:
	case _gl41list_:
	case _vk12list_:
		button_read_bywnd(ent,foot, (void*)wnd,area);
		break;
	default:
		button_read_bycam(ent,foot, stack,sp, arg,key);
	}
}
static void button_giving(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct event* ev = buf;
	if(0x2b70 == ev->what)give_data_into_peer(ent,_evto_, stack,sp, 0,0, "1",1);
}
static void button_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void button_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void button_search(_obj* act)
{
}
static void button_modify(_obj* act)
{
}
static void button_delete(_obj* act)
{
}
static void button_create(_obj* act, u8* arg, int argc, u8** argv)
{
	if(0 == arg)arg = (void*)"button";

	int j;
	u8* dst = act->priv_256b;
	for(j=0;j<0x100;j++){
		if(arg[j] < 0x20)break;
		dst[j] = arg[j];
	}
	dst[j] = 0;
}




void button_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('b', 'u', 't', 't', 'o', 'n', 0, 0);

	p->oncreate = (void*)button_create;
	p->ondelete = (void*)button_delete;
	p->onsearch = (void*)button_search;
	p->onmodify = (void*)button_modify;

	p->onattach = (void*)button_attach;
	p->ondetach = (void*)button_detach;
	p->ontaking = (void*)button_taking;
	p->ongiving = (void*)button_giving;
}
